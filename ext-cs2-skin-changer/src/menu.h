#include "SDK/weapon/C_EconEntity.h"
#include "../ext/skindb.h"

#include "window/window.hpp"

void RenderWeapons(const WeaponsEnum& CurrentType)
{
    std::vector<SkinInfo> skins = skindb->GetWeapons(CurrentType);

    static char search_buffer[64] = "";
    static std::vector<int> filtered_indices;

    static int SelectedSkinIndex = -1;

    SkinInfo currentSkin = vInv->GetSkin(CurrentType);

    if (SelectedSkinIndex == -1 && currentSkin.Paint != 0)
    {
        for (int i = 0; i < skins.size(); i++)
        {
            if (skins[i].Paint == currentSkin.Paint)
            {
                SelectedSkinIndex = i;
                break;
            }
        }
    }

    ImGui::InputText("Search", search_buffer, IM_ARRAYSIZE(search_buffer));
    ImGui::BeginChild("SkinsList", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

    filtered_indices.clear();

    for (int i = 0; i < skins.size(); i++)
    {
        std::string name = skins[i].name;
        std::string query = search_buffer;

        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);

        if (query.empty() || name.find(query) != std::string::npos)
            filtered_indices.push_back(i);
    }

    for (int idx : filtered_indices)
    {
        bool is_selected = (SelectedSkinIndex == idx);
        if (ImGui::Selectable(skins[idx].name.c_str(), is_selected))
        {
            SelectedSkinIndex = idx;

            SkinInfo newSkin;
            newSkin.Paint = skins[idx].Paint;
            newSkin.name = skins[idx].name;
            newSkin.bUsesOldModel = skins[idx].bUsesOldModel;
            newSkin.weaponType = CurrentType;

            vInv->AddSkin(newSkin);
        }

        if (is_selected)
            ImGui::SetItemDefaultFocus();
    }

    ImGui::EndChild();
}

void RenderAddedSkins()
{

}


bool MenuOpen = true;
void RenderMenu(const WeaponsEnum& def)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		MenuOpen = !MenuOpen;

	if (!MenuOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("CS2 Skin Changer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

	// Sidebar tabs
	static int active_tab = 0; // 0: Weapon, 1: Gloves, 2: Knife, 3: Agents, 4: Misc

	ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);
	{
		if (ImGui::Button("Skins", ImVec2(100, 30))) active_tab = 0;
		if (ImGui::Button("AddedSkins", ImVec2(100, 30))) active_tab = 1;
		if (ImGui::Button("Misc", ImVec2(100, 30))) active_tab = 4;
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Main content area
	ImGui::BeginChild("Content", ImVec2(0, 0), false);
	{
		switch (active_tab)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}