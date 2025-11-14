#include "menu.h"
#include "skins.h"
#include "window/window.hpp"

int main()
{
    skindb->Dump();

    overlay::Setup();

    std::thread MenuThread(MenuThreadFunc);
	MenuThread.detach();

	std::cout << "Discord Overlay Hooked!" << std::endl;
	std::cout << "CS2 Skin Changer Loaded!" << std::endl;

    while (true)
    { 
		Sleep(1);

        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t weapon = mem->Read<uintptr_t>(localPlayer + Offsets::m_pClippingWeapon);
        const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
        CurrentWeaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
        if (!localPlayer || !weapon || !item || !CurrentWeaponDef)
            continue;

        //std::cout << std::hex << weapon << std::endl;
        //std::cout << std::hex << localPlayer << std::endl;
        //std::cout << std::dec << GetEntityHandle(weapon) << std::endl;
        //const uintptr_t item = GetHudArms() + Offsets::m_AttributeManager + Offsets::m_Item;
        //std::cout << std::dec << mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex) << std::endl;
        //mem->Write<bool>(localPlayer + Offsets::m_EconGloves + Offsets::m_bRestoreCustomMaterialAfterPrecache, true);
        ////continue;
        //exit(0);
		////OnAgent(localPlayer);
        ////exit(0);
		//OnGloves(localPlayer + Offsets::m_EconGloves);
		////OnGloves(localPlayer);
        //exit(0);
        ////continue;

        if (IsMeleeWeapon(weapon) && bKnife)
            OnMelee(weapon);
        else if(bSkin)
            OnWeapon(weapon);

        if (ShouldUpdateWeapon)
            UpdateWeapon(weapon);
            
    }

    overlay::CloseOverlay();

    return 0;
}