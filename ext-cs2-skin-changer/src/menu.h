#include "SDK/weapon/C_EconEntity.h"

#include "../ext/skindb.h"

#include "window/window.hpp"

#include "config.h"

static WeaponsEnum CurrentWeaponDef;

static int selectedSkinIndex = 0;
static uint16_t selectedMusicKitIndex = static_cast<uint16_t>(MusicKit::CounterStrike2);

void RenderMusicKitTab()
{
    const std::vector<MusicKit_t>& availableMusicKits = musicKits;

    // Safety: no music kits
    if (availableMusicKits.empty())
    {
        if (ImGui::BeginCombo("Select Music Kit", ""))
            ImGui::EndCombo();
        return;
    }

    // Sync selected index from current equipped kit
    for (uint16_t i = 0; i < availableMusicKits.size(); i++)
    {
        if (availableMusicKits[i].id == skinManager->MusicKit.id)
        {
            selectedMusicKitIndex = i;
            break;
        }
    }

    // Clamp just in case
    if (selectedMusicKitIndex < 0 ||
        selectedMusicKitIndex >= static_cast<int>(availableMusicKits.size()))
    {
        selectedMusicKitIndex = 0;
    }

    const char* preview = availableMusicKits[selectedMusicKitIndex].name.c_str();
    if (ImGui::BeginCombo("Select Music Kit", preview))
    {
        for (int i = 0; i < static_cast<int>(availableMusicKits.size()); i++)
        {
            bool isSelected = (selectedMusicKitIndex == i);
            if (ImGui::Selectable(availableMusicKits[i].name.c_str(), isSelected))
                selectedMusicKitIndex = i;

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Apply selected music kit
    // index 0 = default / no kit (same idea as skins)
    if (selectedMusicKitIndex != 0)
        skinManager->MusicKit = availableMusicKits[selectedMusicKitIndex];
}

void RenderWeaponTab()
{
    std::vector<SkinInfo_t>& availableSkins = skindb->GetWeaponSkins(CurrentWeaponDef);

	ImGui::Text("Skin");
	ImGui::SameLine();

    // Safety: ensure selectedSkinIndex is valid for this availableSkins
    if (availableSkins.empty()) {
        // no skins for this weapon: show an empty/placeholder preview
        if (ImGui::BeginCombo("##Select Skin", "")) {
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

	SkinInfo_t& current = availableSkins[selectedSkinIndex];

	int& currentPaintKit = skindb->skinApplied[CurrentWeaponDef];
	currentPaintKit = current.Paint;

	SkinInfo_t* cfgCurrent = nullptr;
	for (SkinInfo_t& si : skindb->skinSettings[CurrentWeaponDef]) {
		if (si.Paint == current.Paint) {
			cfgCurrent = &si;
		}
	}

	if (cfgCurrent == nullptr) {
		SkinInfo_t si;
		si.Paint = current.Paint;

		skindb->skinSettings[CurrentWeaponDef].push_back(si);

		for (SkinInfo_t& si : skindb->skinSettings[CurrentWeaponDef]) {
			if (si.Paint == current.Paint) {
				cfgCurrent = &si;
			}
		}
	}

	ImGui::Text("Pattern");
	ImGui::SameLine();
	ImGui::InputInt("##pattern", &(cfgCurrent->temp_pattern), 1, 10);
	ImGui::SameLine();
	if (ImGui::Button("apply")) {
		if (cfgCurrent->temp_pattern >= 0 && cfgCurrent->temp_pattern <= 1000)
			cfgCurrent->pattern = cfgCurrent->temp_pattern;
		else
			cfgCurrent->temp_pattern = cfgCurrent->pattern;
	}

	ImGui::Text("Wear");
	ImGui::SameLine();
	ImGui::SliderFloat("##wear", &(cfgCurrent->wear), 0.01f, 1.0f, "%.2f");
	

    if (selectedSkinIndex != 0) // keep existing semantic for default slot 0
		skinManager->AddSkin(availableSkins[selectedSkinIndex]);

	//if (ImGui::Button("Force Update")) not needed or working
	//	ForceUpdate = true;
}

//void RenderGlovesTab()
//{
//
//}
//
//static int selectedKnifeIndex = 0;
//static bool bKnife = false;
//
//void RenderKnifeTab()
//{
//	ImGui::Checkbox("Knifes", &bKnife);
//
//	if (ImGui::BeginCombo("Select Knife", Knifes[selectedKnifeIndex].name.c_str()))
//	{
//		for (int i = 0; i < Knifes.size(); i++)
//		{
//			bool isSelected = (selectedKnifeIndex == i);
//			if (ImGui::Selectable(Knifes[i].name.c_str(), isSelected))
//				selectedKnifeIndex = i;
//
//			// Set the initial focus when opening the combo (optional)
//			if (isSelected)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//
//	skinManager->ActiveKnife = Knifes[selectedKnifeIndex];
//}

void SetupImGuiStyle()
{
	// Comfy style by Giuseppe from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.1f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(30.0f, 30.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 10.0f;
	style.PopupBorderSize = 0.0f;
	style.FramePadding = ImVec2(5.0f, 3.5f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 5.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 15.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 5.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.360515f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.42352942f, 0.38039216f, 0.57254905f, 0.5493562f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38039216f, 0.42352942f, 0.57254905f, 0.54901963f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25882354f, 0.25882354f, 0.25882354f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23529412f, 0.23529412f, 0.23529412f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.29411766f, 0.29411766f, 0.29411766f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.29411766f, 0.29411766f, 0.29411766f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.8156863f, 0.77254903f, 0.9647059f, 0.54901963f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509804f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 0.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.29411766f, 0.29411766f, 0.29411766f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.61960787f, 0.5764706f, 0.76862746f, 0.54901963f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.42352942f, 0.38039216f, 0.57254905f, 0.54901963f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.42352942f, 0.38039216f, 0.57254905f, 0.2918455f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433478f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7372549f, 0.69411767f, 0.8862745f, 0.54901963f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

int menu_open = true;

void RenderMenu()
{
	std::vector<SkinInfo_t>& availableSkins = skindb->GetWeaponSkins(CurrentWeaponDef);
	selectedSkinIndex = skinManager->GetSkinIndexFromArray(availableSkins, skinManager->GetSkin(CurrentWeaponDef));

	SkinInfo_t& current = availableSkins[selectedSkinIndex];

	int& currentPaintKit = skindb->skinApplied[CurrentWeaponDef];
	currentPaintKit = current.Paint;

	SkinInfo_t* cfgCurrent = nullptr;
	for (SkinInfo_t& si : skindb->skinSettings[CurrentWeaponDef]) {
		if (si.Paint == current.Paint) {
			cfgCurrent = &si;
		}
	}

	if (cfgCurrent == nullptr) {
		SkinInfo_t si;
		si.Paint = current.Paint;

		skindb->skinSettings[CurrentWeaponDef].push_back(si);

		for (SkinInfo_t& si : skindb->skinSettings[CurrentWeaponDef]) {
			if (si.Paint == current.Paint) {
				cfgCurrent = &si;
				ImGui::Text("well fuck");
			}
		}
	}

	availableSkins[selectedSkinIndex].wear;
	availableSkins[selectedSkinIndex].pattern;
	availableSkins[selectedSkinIndex].temp_pattern;

	ImGui::Text("%f, %d, %d", cfgCurrent->wear, cfgCurrent->pattern, cfgCurrent->temp_pattern);
	ImGui::Text("%f, %d, %d", availableSkins[selectedSkinIndex].wear, availableSkins[selectedSkinIndex].pattern, availableSkins[selectedSkinIndex].temp_pattern);

	if (selectedSkinIndex != 0) // keep existing semantic for default slot 0
		skinManager->AddSkin(availableSkins[selectedSkinIndex]);

	if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
		menu_open = !menu_open;

	if (!menu_open)
		return;

	SetupImGuiStyle();

    static const ImVec2 window_size = ImVec2(1280, 600);
    const ImVec2 display_size = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(display_size.x / 2.f - window_size.x / 2.f, display_size.y / 2.f - window_size.y / 2.f));
	ImGui::Begin("CS2 Skin Changer", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Sidebar tabs
	static int active_tab = 0; // 0: Weapon, 1: Gloves, 2: Knife, 3: Agents, 4: Misc

	ImGui::BeginChild("Sidebar", ImVec2(420, 0), false);
	{
		if (ImGui::Button("Skins", ImVec2(400, 30))) active_tab = 0;
		if (ImGui::Button("MusicKit", ImVec2(400, 30))) active_tab = 1;
		ImGui::NewLine();
		configManager->ConfigPage();
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
            RenderMusicKitTab();
            break;
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void OnFrame()
{
    overlay::Render();
    RenderMenu();
    overlay::EndRender();
}

//bool MenuOpen = true;
void MenuThread()
{
	while (true)
	{
		Sleep(10);

		//if (GetAsyncKeyState(VK_INSERT) & 1)
		//	MenuOpen = !MenuOpen;
        //
		//if (!MenuOpen)
		//	continue;

        OnFrame();
	}
}

void UpdateActiveMenuDef(const uintptr_t localPlayer)
{
	const uintptr_t activeWeapon = mem.Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);

	const uintptr_t activeItem = activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item;

	CurrentWeaponDef = mem.Read<WeaponsEnum>(activeItem + Offsets::m_iItemDefinitionIndex);
}

void InitMenu(const bool autoThread = false)
{
	overlay::Setup();
	
    if (autoThread)
    {
        std::thread menuthread(MenuThread);
        menuthread.detach();
    }

	std::cout << "Menu Setup\n";
}