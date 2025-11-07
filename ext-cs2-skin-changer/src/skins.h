#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/dwEntityListManager.h"
#include "SDK/entity/C_CS2HudModelArms.h"

#include "SDK/MurmurHash2/MurmurHash2.h"

void OnWeapon(const uintptr_t& pWeapon)
{
    const uintptr_t hudWeapon = GetHudWeapon(pWeapon);
    const uintptr_t item = pWeapon + Offsets::m_AttributeManager + Offsets::m_Item;

    CurrentWeaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
    //SkinInfo_t activeSkin = GetActiveSkin(static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex)));
    SkinInfo_t activeSkin = skinManager->GetSkin(CurrentWeaponDef);
    if (!activeSkin.Paint)
        return;

    if (mem->Read<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit) != activeSkin.Paint)
    {
        SetMeshMask(pWeapon, 1 + activeSkin.bUsesOldModel);
        SetMeshMask(hudWeapon, 1 + activeSkin.bUsesOldModel);

        mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
        mem->Write<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit, activeSkin.Paint);

        UpdateWeapon(pWeapon);
    }
}

void OnGloves(const uintptr_t& pGloves)
{
    //std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask) << std::endl;
    //std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + 0x108) + 0x10) << std::endl;
    //

	//const uintptr_t pViewModel = GetHudArms();


	//std::cout << std::hex << pViewModel << std::endl;
    //exit(0);

    mem->Write<uint32_t>(pGloves + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
    mem->Write<uint16_t>(pGloves + Offsets::m_iItemDefinitionIndex, 5030);
    AddAttribute(pGloves, Attributes::Skin, 10048.f);

    //SetMeshMask(GetHudArms(), 1);

    mem->Write<bool>(GetLocalPlayer() + Offsets::m_bInitialized, true);
    mem->Write<bool>(GetLocalPlayer() + Offsets::m_bNeedToReApplyGloves, true);
    
    exit(0);

    std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask) << std::endl;
    std::cout << mem->Read<uint64_t>(mem->Read<uintptr_t>(mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + 0x108) + 0x10) << std::endl;
    std::cout << std::hex << mem->Read<uintptr_t>(GetHudArms() + Offsets::m_pGameSceneNode) + Offsets::m_modelState + Offsets::m_MeshGroupMask << std::endl;
}

void OnMelee(const uintptr_t& pKnife)
{
    const uintptr_t item = pKnife + Offsets::m_AttributeManager + Offsets::m_Item;
    std::string& model = skinManager->ActiveKnife.model;
    CurrentWeaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
    const uint16_t definition = skinManager->ActiveKnife.defIndex;
    if (!definition)
        return;

    const std::string str = std::to_string(definition);
    const uint64_t targetSubclassID = MurmurHash2LowerCaseA(str.c_str(), str.length(), STRINGTOKEN_MURMURHASH_SEED);

    //static SkinInfo_t activeSkin = SkinInfo_t{ 409, false, "", WeaponsEnum::none };
    static SkinInfo_t activeSkin = SkinInfo_t{ 618, false, "", WeaponsEnum::none };

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

        Sleep(100);

        SetMeshMask(pKnife, 1);
        SetMeshMask(hudWeapon, 1);

        UpdateWeapon(pKnife);
    }

    if (mem->Read<int32_t>(pKnife + Offsets::m_nFallbackPaintKit) != activeSkin.Paint)
    {
        Sleep(100);

        mem->Write<bool>(item + Offsets::m_bRestoreCustomMaterialAfterPrecache, true);

        mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
        mem->Write<int32_t>(pKnife + Offsets::m_nFallbackPaintKit, activeSkin.Paint);

        UpdateWeapon(pKnife);
        Sleep(100);
        UpdateWeapon(pKnife);

		SetMeshMask(pKnife, 1);
    }
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