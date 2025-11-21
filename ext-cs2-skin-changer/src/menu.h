#include "SDK/weapon/C_EconEntity.h"
#include "../ext/skindb.h"

#include "window/window.hpp"

static WeaponsEnum CurrentWeaponDef;

static int selectedSkinIndex = 0;

void RenderWeaponTab()
{
    const std::vector<SkinInfo_t> availableSkins = skindb->GetWeaponSkins(CurrentWeaponDef);

    // Safety: ensure selectedSkinIndex is valid for this availableSkins
    if (availableSkins.empty()) {
        // no skins for this weapon: show an empty/placeholder preview
        if (ImGui::BeginCombo("Select Skin", "")) {
            ImGui::EndCombo();
        }
        return;
    }

    //if (selectedSkinIndex < 0 || selectedSkinIndex >= static_cast<int>(availableSkins.size()))
    //    selectedSkinIndex = 0; // clamp or reset when changing weapon

	selectedSkinIndex = skinManager->GetSkinIndexFromArray(availableSkins, skinManager->GetSkin(CurrentWeaponDef));

    const char* preview = availableSkins[selectedSkinIndex].name.c_str();
    if (ImGui::BeginCombo("Select Skin", preview))
    {
        for (int i = 0; i < static_cast<int>(availableSkins.size()); i++)
        {
            bool isSelected = (selectedSkinIndex == i);
            if (ImGui::Selectable(availableSkins[i].name.c_str(), isSelected))
                selectedSkinIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selectedSkinIndex != 0) // keep existing semantic for default slot 0
		skinManager->AddSkin(availableSkins[selectedSkinIndex]);
}

void RenderGlovesTab()
{

}

static int selectedKnifeIndex = 0;
static bool bKnife = false;

void RenderKnifeTab()
{
	ImGui::Checkbox("Knifes", &bKnife);

	if (ImGui::BeginCombo("Select Knife", Knifes[selectedKnifeIndex].name.c_str()))
	{
		for (int i = 0; i < Knifes.size(); i++)
		{
			bool isSelected = (selectedKnifeIndex == i);
			if (ImGui::Selectable(Knifes[i].name.c_str(), isSelected))
				selectedKnifeIndex = i;

			// Set the initial focus when opening the combo (optional)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	skinManager->ActiveKnife = Knifes[selectedKnifeIndex];
}

bool MenuOpen = true;
void RenderMenu()
{
	//if (GetAsyncKeyState(VK_INSERT) & 1)
	//	MenuOpen = !MenuOpen;
    //
	//if (!MenuOpen)
	//	return;

	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("CS2 Skin Changer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

	// Sidebar tabs
	static int active_tab = 0; // 0: Weapon, 1: Gloves, 2: Knife, 3: Agents, 4: Misc

	ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);
	{
		if (ImGui::Button("Skins", ImVec2(100, 30))) active_tab = 0;
		//if (ImGui::Button("Gloves", ImVec2(100, 30))) active_tab = 1;
		if (ImGui::Button("Knifes", ImVec2(100, 30))) active_tab = 2;
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Main content area
	ImGui::BeginChild("Content", ImVec2(0, 0), false);
	{
		switch (active_tab)
		{
		case 0:
			RenderWeaponTab();
			break;
		case 1:
			RenderGlovesTab();
			break;
		case 2:
			RenderKnifeTab();
			break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void MenuThreadFunc()
{
	while (true)
	{
		Sleep(1);
		overlay::Render();
		RenderMenu();
		overlay::EndRender();
	}
}