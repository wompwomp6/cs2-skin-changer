#include "menu.h"
#include "window/window.hpp"

#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/dwEntityListManager.h"
#include "SDK/entity/C_CS2HudModelArms.h"

#include "SDK/MurmurHash2/MurmurHash2.h"

#include "../ext/wcl.h"

SkinInfo_t GetActiveSkin(const WeaponsEnum& def)
{
    switch (def)
    {
    case WeaponsEnum::UspS:
        return SkinInfo_t{ 653, true };//504
    case WeaponsEnum::Glock:
        return SkinInfo_t{ 1208, false };
    case WeaponsEnum::Awp:
        return SkinInfo_t{ 344, true };
    case WeaponsEnum::Ak47:
        return SkinInfo_t{ 801, true };
    case WeaponsEnum::M4A1Silencer:
        return SkinInfo_t{ 644, true };
    case WeaponsEnum::Ssg08:
        return SkinInfo_t{ 222, true };
    }

	return SkinInfo_t{};
}

void OnMelee(const uintptr_t& pKnife)
{
    const uintptr_t item = pKnife + Offsets::m_AttributeManager + Offsets::m_Item;
    std::string& model = ActiveKnife.model;
    const uint16_t definition = ActiveKnife.defIndex;
    if (!definition)
        return;

    const std::string str = std::to_string(definition);
    const uint64_t targetSubclassID = MurmurHash2LowerCaseA(str.c_str(), str.length(), STRINGTOKEN_MURMURHASH_SEED);

    if (mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex) != definition &&
        mem->Read<uint64_t>(pKnife + Offsets::m_nSubclassID) != targetSubclassID)
    {
        mem->Write<uint16_t>(item + Offsets::m_iItemDefinitionIndex, definition);
        mem->Write<uint64_t>(pKnife + Offsets::m_nSubclassID, targetSubclassID);

        UpdateSubclass(pKnife);

        SetModel(pKnife, model);

		Sleep(50);

        const uintptr_t hudWeapon = GetHudWeapon(pKnife);
        SetModel(hudWeapon, model);

        Sleep(1000);


        //UpdateWeapon(pKnife);
    }
}

void OnWeapon(const uintptr_t& pWeapon)
{
    const uintptr_t hudWeapon = GetHudWeapon(pWeapon);
    const uintptr_t item = pWeapon + Offsets::m_AttributeManager + Offsets::m_Item;

	//SkinInfo_t activeSkin = GetActiveSkin(static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex)));
    SkinInfo_t activeSkin = GetSkin(static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex)));

    if (mem->Read<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit) != activeSkin.Paint)
    {
        SetMeshMask(pWeapon, 1 + activeSkin.bUsesOldModel);
        SetMeshMask(hudWeapon, 1 + activeSkin.bUsesOldModel);

        mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
        mem->Write<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit, activeSkin.Paint);
    }

    UpdateWeapon(pWeapon);
}
 
void OnGloves(const uintptr_t& pGloves)
{
    //std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask) << std::endl;
    //std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + 0x108) + 0x10) << std::endl;
    //
    //mem->Write<uint32_t>(pGloves + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
    //mem->Write<uint16_t>(pGloves + Offsets::m_iItemDefinitionIndex, 5030);
    //AddAttribute(pGloves, Attributes::Skin, 10048.f);
    
    //SetMeshMask(GetHudArms(), 1);

    //mem->Write<bool>(pGloves + Offsets::m_bInitialized, true);
    //mem->Write<bool>(GetLocalPlayer() + Offsets::m_bNeedToReApplyGloves, true);
    //
    std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask) << std::endl;
    std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + 0x108) + 0x10) << std::endl;
    std::cout << std::hex << mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask << std::endl;
}

void OnAgent(const uintptr_t& pPawn)
{
    std::cout << std::hex << mem->Read<uint64_t>(pPawn + Offsets::m_nSubclassID) << std::endl;
    std::cout << std::hex << mem->Read<uint64_t>(GetHudArms() + Offsets::m_nSubclassID) << std::endl;

    return;
    SetModel(GetHudArms(), "phase2/characters/models/ctm_fbi/ctm_fbi_varianta_ag2.vmdl"); //works

	Sleep(50);

	SetModel(pPawn, "characters/models/ctm_fbi/ctm_fbi_varianta.vmdl"); // does not work

	//only sets viewmodel not body
}

int main()
{
    skindb->Dump();

    overlay::Setup();

	std::cout << "Discord Overlay Hooked!" << std::endl;
	std::cout << "CS2 Skin Changer Loaded!" << std::endl;

    while (true)
    { 
		Sleep(1);

		overlay::Render();

        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        if (!localPlayer || !weapon)
            continue;

        RenderMenu(weapon);

		//OnAgent(localPlayer);
        //exit(0);
		//OnGloves(localPlayer + Offsets::m_EconGloves);
        //exit(0);
        //continue;

        if (IsMeleeWeapon(weapon))
        {
            if(bKnife)
                OnMelee(weapon);
        }
        else
        {
            OnWeapon(weapon);
        }

		overlay::EndRender();
    }

    overlay::CloseOverlay();

    return 0;
}