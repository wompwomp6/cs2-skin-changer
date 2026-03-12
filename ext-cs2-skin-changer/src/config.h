#pragma once

#include "../ext/json.hpp"
#include "../ext/skindb.h"
#include "../ext/ImGui 1.90/imgui.h"
#include <fstream>
#include <string>
#include <filesystem>

class ConfigManager {
private:
	static const int nameSize = 64;
public:
	char nameBuffer[nameSize];

	void ConfigPage() {
		ImGui::Text("Config");
		ImGui::InputText("##new_name", nameBuffer, nameSize);
		if (ImGui::Button("Save Config"))
			SaveConfig(std::string(nameBuffer));
		if (ImGui::Button("Load Config"))
			LoadConfig(std::string(nameBuffer));
	}

	void LoadConfig(std::string name) {
		std::filesystem::create_directories("configs");

		nlohmann::json in;
		std::ifstream i("configs/" + name + ".json");
		if (!i.is_open())
			return;

		i >> in;

		if (in.contains("skinSettings"))
		for (auto& [key, value] : skindb->skinSettings) {
			std::string str_key = std::to_string(key);
			if (!in["skinSettings"].contains(str_key))
				continue;

			for (auto& v : value) {
				std::string str_pk = std::to_string(v.Paint);

				if (in["skinSettings"][str_key].contains(str_pk)) {
					v.wear = in["skinSettings"][str_key][str_pk].value("Wear", v.wear);
					v.pattern = in["skinSettings"][str_key][str_pk].value("Pattern", v.pattern);
					v.temp_pattern = in["skinSettings"][str_key][str_pk].value("Pattern", v.temp_pattern);
				}
			}
		}

		if (in.contains("skinApplied")) {
			for (auto& [key, value] : in["skinApplied"].items()) {
				WeaponsEnum we_key = (WeaponsEnum)(std::stoi(key));
				skindb->skinApplied[we_key] = value;
			}
		}

		ForceUpdate = true;
		ForceLoadSkinConfig = true;
	}

	void SaveConfig(std::string name) {
		std::filesystem::create_directories("configs");

		nlohmann::json out;
		for (const auto& [key, value] : skindb->skinSettings) {
			std::string str_key = std::to_string(key);
			out["skinSettings"][str_key] = nlohmann::json();
			for (auto& v : value) {
				nlohmann::json obj;
				obj["Wear"] = v.wear;
				obj["Pattern"] = v.pattern;

				out["skinSettings"][str_key][std::to_string(v.Paint)] = obj;
			}
		}

		for (const auto& [key, value] : skindb->skinApplied) {
			std::string str_key = std::to_string(key);
			out["skinApplied"][str_key] = value;
		}

		std::ofstream o("configs/" + name + ".json");
		if (o.is_open()) {
			o << out.dump(4);
		}
	}

	ConfigManager() {
		memset(nameBuffer, 0, nameSize);
	}
};

inline ConfigManager* configManager = new ConfigManager();