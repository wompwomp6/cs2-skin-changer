#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

struct SkinInfo {
    int Paint;  
    std::string name;
    bool bUsesOldModel; 
    WeaponsEnum weaponType;
};

class VInv
{
private:
    std::vector<SkinInfo>* skins = new std::vector<SkinInfo>();
public:
    void AddSkin(SkinInfo& Skin)
    {
        for (auto& skin : *skins)
        {
            if (skin.weaponType == Skin.weaponType)
            {
                skin = Skin;
                return;
            }     
        }

        skins->push_back(Skin);
    }

    SkinInfo GetSkin(WeaponsEnum def)
    {
        if (!skins->empty())
            return SkinInfo();

        for (const auto& skin : *skins)
        {
            if (skin.weaponType == def)
                return skin;
        }

        return SkinInfo();
    }
};
VInv* vInv = new VInv();

class CSkinDB {
private:
    std::vector<SkinInfo>* knifeSkins = new std::vector<SkinInfo>();
    std::vector<SkinInfo>* gloveSkins = new std::vector<SkinInfo>();
    std::vector<SkinInfo>* weaponSkins = new std::vector<SkinInfo>();

    std::string GetStringSafe(const nlohmann::json& j, const std::string& key) {
        if (!j.contains(key)) return "";
        if (j[key].is_string()) return j[key].get<std::string>();
        if (j[key].is_object() && j[key].contains("en")) return j[key]["en"].get<std::string>();
        return "";
    }

    int GetPaintIndexSafe(const nlohmann::json& skin) {
        if (!skin.contains("paint_index"))
            return 0;
        if (skin["paint_index"].is_number_integer())
            return skin["paint_index"].get<int>();
        if (skin["paint_index"].is_string()) {
            try { return std::stoi(skin["paint_index"].get<std::string>()); }
            catch (...) { return 0; }
        }
        return 0;
    }

    std::vector<std::string> knifeTypes = {
                    "Bayonet", "Classic Knife", "Flip Knife", "Gut Knife",
                    "Karambit", "M9 Bayonet", "Huntsman Knife", "Falchion Knife",
                    "Bowie Knife", "Butterfly Knife", "Shadow Daggers", "Paracord Knife",
                    "Survival Knife", "Ursus Knife", "Navaja Knife", "Nomad Knife",
                    "Stiletto Knife", "Talon Knife", "Skeleton Knife", "Kukri Knife"
    };

    std::vector<std::string> gloveTypes = {
        "Bloodhound Gloves", "Broken Fang Gloves", 
        "Driver Gloves", "Hand Wraps", 
        "Hydra Gloves", "Moto Gloves", 
        "Specialist Gloves", "Sport Gloves"
    };

    WeaponsEnum GetDefPerString(const std::string& name)
    {
        static const std::unordered_map<std::string, WeaponsEnum> weaponMap = {
            {"AK-47", WeaponsEnum::Ak47},
            {"AUG", WeaponsEnum::Aug},
            {"AWP", WeaponsEnum::Awp},
            {"PP-Bizon", WeaponsEnum::Bizon},
            {"PP-CZ75-Auto", WeaponsEnum::Cz65A},
            {"Desert Eagle", WeaponsEnum::Deagle},
            {"Dual Berettas", WeaponsEnum::Elite},
            {"FAMAS", WeaponsEnum::Famas},
            {"Five-SeveN", WeaponsEnum::FiveSeven},
            {"G3SG1", WeaponsEnum::G3Sg1},
            {"Galil AR", WeaponsEnum::Sg556},
            {"Glock-18", WeaponsEnum::Glock},
            {"P2000", WeaponsEnum::P200},
            {"M249", WeaponsEnum::M249},
            {"M4A1-S", WeaponsEnum::M4A1Silencer},
            {"M4A4", WeaponsEnum::M4A4},
            {"MAC-10", WeaponsEnum::Mac10},
            {"MAG-7", WeaponsEnum::Mag7},
            {"MP5-SD", WeaponsEnum::Ump45},
            {"MP7", WeaponsEnum::Mp7},
            {"MP9", WeaponsEnum::Mp9},
            {"Negev", WeaponsEnum::Negev},
            {"Nova", WeaponsEnum::Nova},
            {"XM1014", WeaponsEnum::Xm1014},
            {"USP-S", WeaponsEnum::UspS},
            {"Tec-9", WeaponsEnum::Tec9},
            {"Zeus x27", WeaponsEnum::Zeus},
            {"SSG 08", WeaponsEnum::Ssg08},
            {"SG 553", WeaponsEnum::Sg556},
            {"SCAR-20", WeaponsEnum::Scar20},
            {"Sawed-Off", WeaponsEnum::Sawedoof},
            {"R8 Revolver", WeaponsEnum::Revolver},
            {"P90", WeaponsEnum::P90},
            {"P250", WeaponsEnum::p250}
        };

        for (const auto& [key, value] : weaponMap)
        {
            if (name.find(key) != std::string::npos)
                return value;
        }

        return WeaponsEnum(); // or some default/invalid value
    }

public:
    void Dump() {
        CURL* curl = curl_easy_init();
        if (!curl) return;

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/ByMykel/CSGO-API/main/public/api/en/skins.json");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return;
        }
        curl_easy_cleanup(curl);

        try {
            auto jsonData = nlohmann::json::parse(readBuffer);

            for (auto& skin : jsonData) {
                SkinInfo info;
                info.Paint = GetPaintIndexSafe(skin);
                info.name = GetStringSafe(skin, "name");
                info.weaponType = GetDefPerString(info.name);

                if (skin.contains("legacy_model") && skin["legacy_model"].is_boolean()) {
                    info.bUsesOldModel = !skin["legacy_model"].get<bool>();
                }

                std::string weaponType = GetStringSafe(skin, "weapon");
                bool isKnife = false;
                bool isGlove = false;

                for (auto& k : knifeTypes)
                    if (info.name.find(k) != std::string::npos) {
                        isKnife = true;
                        break;
                    }

                for (auto& g : gloveTypes)
                    if (info.name.find(g) != std::string::npos) {
                        isGlove = true;
                        break;
                    }

                if (isKnife)
                {
                    knifeSkins->push_back(info);
                    continue;
                }
                if (isGlove)
                {
                    gloveSkins->push_back(info);
                    continue;
                }
                weaponSkins->push_back(info);     
            }
        }
        catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }

    std::vector<SkinInfo> GetWeapons(WeaponsEnum type = WeaponsEnum::none)
    {
        std::vector<SkinInfo> results;

        for (const auto& skin : *weaponSkins)
        {
            if (type != WeaponsEnum::none && skin.weaponType != type)
                continue;

            results.push_back(skin);
        }

        return results;
    }
};
CSkinDB* skindb = new CSkinDB();