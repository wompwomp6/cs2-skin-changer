#include "menu.h"
#include "window/window.hpp"

#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/dwEntityListManager.h"
#include "SDK/entity/C_CS2HudModelArms.h"

void Patches()
{
    mem->Patch(Sigs::SetFallBackDataPatch, 3);
    mem->Patch(Sigs::RegenerateWeaponSkinsPatch, 7);
}

int main()
{
    std::cout << std::hex << mem->Read<uintptr_t>(mem->Read<uintptr_t>(mem->GetModuleBase(L"client.dll") + Offsets::dwLocalPlayerPawn) + Offsets::m_pClippingWeapon) << std::endl;
    //std::cout << std::hex << Sigs::CEconItemAttributeVtable << std::endl;
    return 0;
    //skindb->Dump();

    //overlay::SetupWindow(L"cs2 ext skin changer", L"cs2_overlay");
    //overlay::CreateDeviceD3D(overlay::Window);
 
    Patches();

    //std::cout << "[SkinChanger] Loaded Press Insert For Menu\n";
    std::cout << "[SkinChanger] Loaded :)\n";
    Beep(400, 500);

    const auto client = mem->GetModuleBase(L"client.dll");
    
    SkinInfo dlore = SkinInfo(344, "", true, Awp);
    SkinInfo neonoir = SkinInfo(653, "", true, UspS);
    SkinInfo FireSerpent = SkinInfo(180, "", true, Ak47);
    SkinInfo m4fade = SkinInfo(1177, "", false, M4A1Silencer);
    SkinInfo glockfade = SkinInfo(38, "", false, Glock);
    SkinInfo deagleblaze = SkinInfo(37, "", false, Deagle);
    vInv->AddSkin(dlore);
    vInv->AddSkin(neonoir);
    vInv->AddSkin(FireSerpent);
    vInv->AddSkin(m4fade);
    vInv->AddSkin(glockfade);
    vInv->AddSkin(deagleblaze);

    while (true)
    {
        Sleep(10);

        const auto localPlayer = mem->Read<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
        const auto weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        const auto item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
        const auto weaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
        if (!weapon || !item || !weaponDef)
            continue;
        
        //overlay::Render();
        //RenderMenu(weaponDef);
        //overlay::EndRender();
        
        const SkinInfo& ActiveSkin = vInv->GetSkin(weaponDef);
        //const SkinInfo ActiveSkin = SkinInfo(653, "usp", true, UspS);
        //const SkinInfo ActiveSkin = SkinInfo(796, "usp", false, UspS);
        //const SkinInfo ActiveSkin = SkinInfo(344, "", true, Awp);
        //const SkinInfo ActiveSkin = SkinInfo(917, "", false, Awp);
        if (weaponDef != ActiveSkin.weaponType)
            continue;

        if (mem->Read<uint32_t>(weapon + Offsets::m_nFallbackPaintKit) != ActiveSkin.Paint)
        {
            SetMeshMask(weapon, 1 + ActiveSkin.bUsesOldModel);
            SetMeshMask(GetHudWeapon(localPlayer, weapon), 1 + ActiveSkin.bUsesOldModel);
            
            mem->Write<uint32_t>(item + Offsets::m_iAccountID, mem->Read<uint32_t>(weapon + Offsets::m_OriginalOwnerXuidLow));
            mem->Write<ItemIds>(item + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
            mem->Write<int32_t>(weapon + Offsets::m_nFallbackPaintKit, ActiveSkin.Paint);

            //mem->WriteString(item + Offsets::m_szCustomNameOverride, "Niggawp");

            //mem->Write<int32_t>(weapon + Offsets::m_nFallbackStatTrak, 1337);
            
            Sleep(300);
            
            UpdateSkin();
        }
    }  
}