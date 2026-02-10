#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "../curl/curl.h"
#include "json.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

#include <Windows.h>
#include <winhttp.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "../src/SDK/weapon/weapon.h"
#include "../src/SDK/musicKits.h"

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "shell32.lib")

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::unordered_map<uint16_t, std::string> KnifeModels = {
    {500, "phase2/weapons/models/knife/knife_bayonet/weapon_knife_bayonet_ag2.vmdl"},
    {503, "phase2/weapons/models/knife/knife_css/weapon_knife_css_ag2.vmdl"},
    {505, "phase2/weapons/models/knife/knife_flip/weapon_knife_flip_ag2.vmdl"},
    {506, "phase2/weapons/models/knife/knife_gut/weapon_knife_gut_ag2.vmdl"},
    {507, "phase2/weapons/models/knife/knife_karambit/weapon_knife_karambit_ag2.vmdl"},
    {508, "phase2/weapons/models/knife/knife_m9/weapon_knife_m9_ag2.vmdl"},
    {509, "phase2/weapons/models/knife/knife_tactical/weapon_knife_tactical_ag2.vmdl"},
    {512, "phase2/weapons/models/knife/knife_falchion/weapon_knife_falchion_ag2.vmdl"},
    {514, "phase2/weapons/models/knife/knife_bowie/weapon_knife_bowie_ag2.vmdl"},
    {515, "phase2/weapons/models/knife/knife_butterfly/weapon_knife_butterfly_ag2.vmdl"},
    {516, "phase2/weapons/models/knife/knife_push/weapon_knife_push_ag2.vmdl"},
    {517, "phase2/weapons/models/knife/knife_cord/weapon_knife_cord_ag2.vmdl"},
    {518, "phase2/weapons/models/knife/knife_canis/weapon_knife_canis_ag2.vmdl"},
    {519, "phase2/weapons/models/knife/knife_ursus/weapon_knife_ursus_ag2.vmdl"},
    {520, "phase2/weapons/models/knife/knife_navaja/weapon_knife_navaja_ag2.vmdl"},
    {521, "phase2/weapons/models/knife/knife_outdoor/weapon_knife_outdoor_ag2.vmdl"},
    {522, "phase2/weapons/models/knife/knife_stiletto/weapon_knife_stiletto_ag2.vmdl"},
    {523, "phase2/weapons/models/knife/knife_talon/weapon_knife_talon_ag2.vmdl"},
    {525, "phase2/weapons/models/knife/knife_skeleton/weapon_knife_skeleton_ag2.vmdl"},
    {526, "phase2/weapons/models/knife/knife_kukri/weapon_knife_kukri_ag2.vmdl"}
};
std::map<uint16_t, std::string> KnifeNames = {
    {500, "Bayonet"},
    {503, "Classic"},
    {505, "Flip"},
    {506, "Gut"},
    {507, "Karambit"},
    {508, "M9 Bayonet"},
    {509, "Huntsman"},
    {512, "Falchion"},
    {514, "Bowie"},
    {515, "Butterfly"},
    {516, "Daggers"},
    {517, "Paracord"},
    {518, "Survival"},
    {519, "Ursus"},
    {520, "Navaja"},
    {521, "Nomad"},
    {522, "Stiletto"},
    {523, "Talon"},
    {525, "Skeleton"},
    {526, "Kukri"}
};

class Knife_t
{
public:
    Knife_t(const uint16_t def = 0)
    {
        defIndex = def;
        if (def)
        {
            name = KnifeNames.at(def);
            model = KnifeModels.at(def);
        }  
    }

    uint16_t defIndex;
	std::string name;
	std::string model;
    
};

std::vector<Knife_t> Knifes = {
    Knife_t(500), Knife_t(503), Knife_t(505), Knife_t(506), Knife_t(507),
    Knife_t(508), Knife_t(509), Knife_t(512), Knife_t(514), Knife_t(515), Knife_t(516),
    Knife_t(517), Knife_t(518), Knife_t(519), Knife_t(520), Knife_t(521),
    Knife_t(522), Knife_t(523), Knife_t(525), Knife_t(526)
};

struct SkinInfo_t {
    int Paint;
    int temp_pattern = 0;
    int pattern = 0;
    float wear = 0.01f;
    bool bUsesOldModel = false;
    bool loaded = false;
    std::string name = "";
    WeaponsEnum weaponType = WeaponsEnum::none;
};

struct Glove_t
{
    uint16_t defIndex;
    int Paint;
};

bool ForceUpdate = false;
bool ForceLoadSkinConfig = false;

class CSkinDB {
private:
    //std::vector<SkinInfo_t> knifeSkins;
    //std::vector<SkinInfo_t> gloveSkins;
    std::vector<SkinInfo_t> weaponSkins;

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
            {"PP-CZ75-Auto", WeaponsEnum::Cz75A},
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

    bool EnsureDirectory(const std::wstring& path)
    {
        return std::filesystem::create_directories(path) || std::filesystem::exists(path);
    }

public:
    void DumpSkindb()
    {
        CURL* curl = curl_easy_init();
        if (!curl) return;

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
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
                SkinInfo_t info;
                info.Paint = GetPaintIndexSafe(skin);
                info.name = GetStringSafe(skin, "name");
                info.weaponType = (WeaponsEnum)skin["weapon"].value("weapon_id", 0);

                if (skin.contains("legacy_model") && skin["legacy_model"].is_boolean()) {
                    info.bUsesOldModel = skin["legacy_model"].get<bool>();
                }

                std::string weaponType = GetStringSafe(skin, "weapon");
                std::cout << weaponType << "\n";

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
                    //knifeSkins.push_back(info);
                    continue;
                }
                if (isGlove)
                {
                    //gloveSkins.push_back(info);
                    continue;
                }
                weaponSkins.push_back(info);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }

        std::cout << "Skindb dumped" << std::endl;
    }

    void DumpSkinEconImages()
    {

        return;

        std::cout << "Econ images dumped" << std::endl;
    }

    inline void Dump() { DumpSkindb(); DumpSkinEconImages(); }

    std::map<WeaponsEnum, std::vector<SkinInfo_t>> skinSettings;
    std::map<WeaponsEnum, int> skinApplied;

    std::vector<SkinInfo_t>& GetWeaponSkins(WeaponsEnum type = WeaponsEnum::none)
    {
        std::vector<SkinInfo_t>& results = skinSettings[type];

        if (results.empty()) {
            SkinInfo_t si;
            si.name = "Vanila";

            results.push_back(si);
        }

        if (type == WeaponsEnum::none)
            return results;

        if (results.size() == 1) {
            for (const auto& skin : weaponSkins)
            {
                if (skin.weaponType != type)
                    continue;

                results.push_back(skin);
            }
        }
        return results;
    }
};
CSkinDB* skindb = new CSkinDB();

class SkinManager
{
public:
    std::vector<SkinInfo_t> Skins;
    Glove_t Gloves = Glove_t();
    Knife_t Knife = Knife_t();
    MusicKit_t MusicKit = MusicKit_t(static_cast<uint16_t>(MusicKit::CounterStrike2), "Counter-Strike 2");

    void AddSkin(SkinInfo_t AddedSkin)
    {
        for (SkinInfo_t& skin : Skins)
        {
            if (skin.weaponType == AddedSkin.weaponType)
            {
                if (skin.Paint == AddedSkin.Paint && skin.wear == AddedSkin.wear && skin.pattern == AddedSkin.pattern)
                    return;

                skin = AddedSkin;
                ForceUpdate = true;
                return;
            }    
        }

        Skins.push_back(AddedSkin);
        ForceUpdate = true;
    }

    SkinInfo_t GetSkin(const WeaponsEnum def)
    {
        for (auto& [weapon, pk] : skindb->skinApplied) {
            if (def == weapon) {
                SkinInfo_t found_skin;
                found_skin.Paint = pk;

                std::vector<SkinInfo_t>& vector = skindb->skinSettings[def];

                for (SkinInfo_t& si : vector) {
                    if (si.Paint == pk)
                    {
                        return si;
                    }
                }
            }
        }

        for (const SkinInfo_t& skin : Skins)
           if (skin.weaponType == def)
             return skin;

        return SkinInfo_t();
    }

    uint16_t GetSkinIndexFromArray(std::vector<SkinInfo_t> WeaponSkins, SkinInfo_t SelectedSkin)
    {
        for (int i = 0; i < WeaponSkins.size(); i++)
        {
            if (WeaponSkins[i].Paint == SelectedSkin.Paint) {
                return i;
            }
        }

		return 0;
    }

    //void PharseJson(//file path here);
	//void ExportJson(//file path here);
};
SkinManager* skinManager = new SkinManager();