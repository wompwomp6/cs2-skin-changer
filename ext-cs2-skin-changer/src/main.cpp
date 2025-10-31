#include "menu.h"
#include "window/window.hpp"

#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/dwEntityListManager.h"
#include "SDK/entity/C_CS2HudModelArms.h"

#include "SDK/vtable.h"

#include "../ext/wcl.h"


void UpdateWeapon(const uintptr_t& weapon)
{
    const uintptr_t& CompositeMaterial = weapon + 0x5F8;
    wcl->CallFunction(Sigs::UpdateComposite,
        {
            CArg{ ASM::RCX, CompositeMaterial },
            CArg{ ASM::dl, true },
        });

    wcl->CallFunction(Sigs::UpdateModel,
        {
            CArg{ ASM::RCX, weapon },
            CArg{ ASM::dl, false },
        });
}

void PostDataUpdate(const uintptr_t& weapon)
{
    wcl->CallFunction(mem->GetVtableFunc(mem->Read<uintptr_t>(weapon), Vtable::UpdatePostData),
        {
            CArg{ ASM::RCX, weapon },
            CArg{ ASM::dl, true },
        });
}


void SetModel(const uintptr_t& weapon, std::string model)
{
    const uintptr_t pModel = mem->Allocate(NULL, MemPage);
    mem->WriteString(pModel, model);

    wcl->CallFunction(Sigs::SetModel,
        {
            CArg{ ASM::RCX, weapon },
            CArg{ ASM::RDX, pModel },
        });

    mem->Free(pModel, MemPage);
}

void UpdateModel(const uintptr_t& weapon)
{
    SetModel(weapon, "");
}

void UpdateSubclass(const uintptr_t& weapon)
{
    wcl->CallFunction(Sigs::SubclassUpdate,
        {
            CArg{ ASM::RCX, weapon },
        });
}

int main()
{
    const uintptr_t localPlayer = mem->Read<uintptr_t>(mem->GetModuleBase(L"client.dll") + Offsets::dwLocalPlayerPawn);
    const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
    const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
    const uintptr_t hudWeapon = GetHudWeapon(localPlayer, weapon);
    
    ////std::cout << std::hex << activeWeapon << std::endl;
    ////std::cout << std::hex << mem->GetVtableFunc(mem->Read<uintptr_t>(activeWeapon), 7) << std::endl;
    ////std::cout << std::hex << mem->GetVtableFunc(mem->Read<uintptr_t>(activeWeapon), 95) << std::endl;
    ////return 0;
    //
    //mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);
    //mem->Write<int32_t>(weapon + Offsets::m_nFallbackPaintKit, 653);
    //
    //PostDataUpdate(hudWeapon);
    //
    ////SetMeshMask(activeWeapon, 2);
    ////SetMeshMask(activeHudWeapon, 2);
    ////
    ////AddAttribute(activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item, Attributes::Skin, 653.f);
    //
    //UpdateWeapon(weapon);
    //
    //
    //
    ////mem->Write<uint32_t>(localPlayer + Offsets::m_EconGloves + Offsets::m_iItemIDHigh, -1);
    ////mem->Write<uint16_t>(localPlayer + Offsets::m_EconGloves + Offsets::m_iItemDefinitionIndex, 5030);
    ////AddAttribute(localPlayer + Offsets::m_EconGloves, Attributes::Skin, 10048.f);
    ////
    ////mem->Write<bool>(localPlayer + Offsets::m_EconGloves + Offsets::m_bInitialized, true);
    ////mem->Write<bool>(localPlayer + Offsets::m_bNeedToReApplyGloves, true);
    //
    //return 0;

    //const uintptr_t activeHudWeaponNode = mem->Read<uintptr_t>(hudWeapon + Offsets::m_pGameSceneNode);

    static const std::map<uint16_t, uint64_t> m_subclassIdMap = {
            {500, 3933374535},
            {503, 3787235507},
            {505, 4046390180},
            {506, 2047704618},
            {507, 1731408398},
            {508, 1638561588},
            {509, 2282479884},
            {512, 3412259219},
            {514, 2511498851},
            {515, 1353709123},
            {516, 4269888884},
            {517, 1105782941},
            {518, 275962944},
            {519, 1338637359},
            {520, 3230445913},
            {521, 3206681373},
            {522, 2595277776},
            {523, 4029975521},
            {524, 2463111489},
            {525, 365028728},
            {526, 3845286452},
    };

    const uint16_t definition = 507;
    const uint64_t targetSubclassID = m_subclassIdMap.at(definition);
    
    //mem->Write<uint16_t>(item + Offsets::m_iItemDefinitionIndex, definition);
    mem->Write<uint64_t>(weapon + Offsets::m_nSubclassID, targetSubclassID);
    
    UpdateSubclass(weapon);
    //
    //Sleep(500);

    std::string model = "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl";
    
    //SetModel(weapon, model);
    //SetModel(hudWeapon, model);

    while (true)
    {
        Sleep(50);

        if (!GetAsyncKeyState(VK_HOME)) continue;

        uintptr_t weapon2 = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);

        UpdateModel(weapon2);
    }

    UpdateModel(weapon);
    //
    //Sleep(50);
    //
    //SetModel(weapon, model);
    //SetModel(hudWeapon, model);

    //std::cout << std::dec << mem->Read<uint64_t>(weapon + Offsets::m_nSubclassID) << std::endl;

    return 0;
    ////skindb->Dump();
    //
    ////overlay::SetupWindow(L"cs2 ext skin changer", L"cs2_overlay");
    ////overlay::CreateDeviceD3D(overlay::Window);
    ////std::cout << "[SkinChanger] Loaded Press Insert For Menu\n";
    //std::cout << "[SkinChanger] Loaded :)\n";
    //Beep(400, 500);
    //
    //const auto client = mem->GetModuleBase(L"client.dll");
    //
    //SkinInfo awp = SkinInfo(344, "", true, Awp);
    //SkinInfo usps = SkinInfo(504, "", true, UspS);
    //SkinInfo ak = SkinInfo(44, "", true, Ak47);
    //SkinInfo m4 = SkinInfo(1177, "", false, M4A1Silencer);
    //SkinInfo glock = SkinInfo(38, "", false, Glock);
    //SkinInfo deagle = SkinInfo(37, "", false, Deagle);
    //vInv->AddSkin(awp);
    //vInv->AddSkin(usps);
    //vInv->AddSkin(ak);
    //vInv->AddSkin(m4);
    //vInv->AddSkin(glock);
    //vInv->AddSkin(deagle);
    //
    //while (true)
    //{
    //    Sleep(10);
    //
    //    const auto localPlayer = mem->Read<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
    //    const auto weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
    //    const auto item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
    //    const auto weaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
    //    if (!weapon || !item || !weaponDef)
    //        continue;
    //    
    //    //overlay::Render();
    //    //RenderMenu(weaponDef);
    //    //overlay::EndRender();
    //    
    //    const SkinInfo& ActiveSkin = vInv->GetSkin(weaponDef);
    //    //const SkinInfo ActiveSkin = SkinInfo(653, "usp", true, UspS);
    //    //const SkinInfo ActiveSkin = SkinInfo(796, "usp", false, UspS);
    //    //const SkinInfo ActiveSkin = SkinInfo(344, "", true, Awp);
    //    //const SkinInfo ActiveSkin = SkinInfo(917, "", false, Awp);
    //    if (weaponDef != ActiveSkin.weaponType)
    //        continue;
    //
    //    if (mem->Read<uint32_t>(weapon + Offsets::m_nFallbackPaintKit) != ActiveSkin.Paint)
    //    {
    //        SetMeshMask(weapon, 1 + ActiveSkin.bUsesOldModel);
    //        SetMeshMask(GetHudWeapon(localPlayer, weapon), 1 + ActiveSkin.bUsesOldModel);
    //        
    //        mem->Write<uint32_t>(item + Offsets::m_iAccountID, mem->Read<uint32_t>(weapon + Offsets::m_OriginalOwnerXuidLow));//setup for statTrack
    //
    //
    //        std::vector<CEconItemAttribute> attributes;
    //        attributes.push_back(itemAttributeManager->MakeItemAttribute(weapon, Paint, (float)ActiveSkin.Paint));
    //        //attributes.push_back(itemAttributeManager->MakeItemAttribute(weapon, Wear, (float) 0.69));
    //        attributes.push_back(itemAttributeManager->MakeItemAttribute(weapon, Pattern, (float) 661));
    //        //attributes.push_back(itemAttributeManager->MakeItemAttribute(weapon, StatTrack, (float) 1337));
    //        
    //        itemAttributeManager->pSwapAttributes(item, attributes);
    //        
    //        mem->Write<int32_t>(weapon + Offsets::m_nFallbackPaintKit, ActiveSkin.Paint);//keep tabs on what skin was applied last because we cleanup the attributes
    //        
    //        UpdateSkin();
    //        
    //        itemAttributeManager->pCleanupSwapAttributes(item);
    //
    //
    //        //mem->WriteString(item + Offsets::m_szCustomNameOverride, "Niggawp");
    //    }
    //}  
}