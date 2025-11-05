#include "SDK/weapon/C_EconEntity.h"
#include "../ext/skindb.h"

#include "window/window.hpp"

static int selectedSkinIndex = 0;
static bool bSkin = false;

void RenderWeaponTab(const uintptr_t& weapon)
{
	const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
	const WeaponsEnum defIndex = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
	const std::vector<SkinInfo_t> availableSkins = skindb->GetWeaponSkins(defIndex);

	selectedSkinIndex = 0;

	ImGui::Checkbox("Skins", &bSkin);

	if (ImGui::BeginCombo("Select Skin", availableSkins[selectedSkinIndex].name.c_str()))
	{
		for (int i = 0; i < availableSkins.size(); i++)
		{
			bool isSelected = (selectedSkinIndex == i);
			if (ImGui::Selectable(availableSkins[i].name.c_str(), isSelected))
				selectedSkinIndex = i;

			// Set the initial focus when opening the combo (optional)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if(selectedSkinIndex)
		AddSkin(availableSkins[selectedSkinIndex]);
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

	ActiveKnife = Knifes[selectedKnifeIndex];
}

bool MenuOpen = true;
void RenderMenu(const uintptr_t& weapon)
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
		if (ImGui::Button("Gloves", ImVec2(100, 30))) active_tab = 1;
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
			break;
		case 1:
			break;
		case 2:
			RenderKnifeTab();
			break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}