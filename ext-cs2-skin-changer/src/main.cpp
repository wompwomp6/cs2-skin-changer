#include "menu.h"
#include "window/window.hpp"

#include "../ext/offsets.h"
#include "../ext/sigs.h"

#include "SDK/entity/entity.h"
#include "SDK/entity/arms.h"

void UpdateSkin()
{
    if (Sigs::SwitchHands)
    {
        mem->CallThread(Sigs::SwitchHands);
        Sleep(50);
        mem->CallThread(Sigs::SwitchHands);
    }

    Sleep(50);

    if (Sigs::RegenerateWeaponSkins)
    {
        mem->CallThread(Sigs::RegenerateWeaponSkins);
    }
}

int main()
{
    skindb->Dump();

    overlay::SetupWindow(L"cs2 ext skin changer", L"cs2_overlay");
    overlay::CreateDeviceD3D(overlay::Window);
 
    if (Sigs::SetFallBackDataPatch)
        mem->Patch(Sigs::SetFallBackDataPatch, 3);

    std::cout << "[SkinChanger] Loaded Press Insert For Menu\n";
    Beep(400, 500);

    const auto client = mem->GetModuleBase(L"client.dll");
    while (true)
    {
        Sleep(10);

        const auto localPlayer = mem->Read<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
        const auto& weaponServices = mem->Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);
        const auto weapon = GetEntityByHandle(mem->Read<uintptr_t>(weaponServices + Offsets::m_hActiveWeapon));
        const auto item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
        const auto weaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
        if (!weapon)
            continue;

        overlay::Render();
        RenderMenu(weaponDef);
        overlay::EndRender();

        //const SkinInfo& ActiveSkin = vInv->GetSkin(weaponDef);
        const SkinInfo ActiveSkin = SkinInfo(653, "usp", true, none);

        std::cout << ActiveSkin.Paint << std::endl;

        if (mem->Read<uint32_t>(weapon + Offsets::m_nFallbackPaintKit) != ActiveSkin.Paint)
        {
            const auto& node = mem->Read<uintptr_t>(GetHudWeapon(localPlayer, weapon) + Offsets::m_pGameSceneNode);
            const auto model = node + Offsets::m_modelState;
            const auto dirtyAttributes = mem->Read<uintptr_t>(model + 0x108);

            mem->Write<uint64_t>(model + Offsets::m_MeshGroupMask, 1 + ActiveSkin.bUsesOldModel);
            mem->Write<uint64_t>(dirtyAttributes + 0x10, 1 + ActiveSkin.bUsesOldModel);
        }
        
        
        
        

        if (GetAsyncKeyState(VK_HOME) & 1)
        {
            mem->Write<uint32_t>(item + Offsets::m_iAccountID, mem->Read<uint32_t>(weapon + Offsets::m_OriginalOwnerXuidLow));//set acc id for st
            mem->Write<ItemIds>(item + Offsets::m_iItemIDHigh, ItemIds::UseFallBackValues);
            mem->Write<int32_t>(weapon + Offsets::m_nFallbackPaintKit, ActiveSkin.Paint);

            Sleep(100);

            UpdateSkin();
        }
    }  
}