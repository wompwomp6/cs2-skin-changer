#include "menu.h"
#include "skins.h"
#include "window/window.hpp"

uint8_t GetIndex(const uintptr_t addy, const uintptr_t weapon)
{
    uint8_t index = 0;
    while (true)
    {
        if (index == -1)
            return 0;

        const uintptr_t naddy = addy + (0x20 * index);
        if (mem->Read<uintptr_t>(naddy) == weapon)
            return index;

        index++;
    }
}

void SetUpdateId(const uintptr_t addy, const uint8_t index)
{
    const uintptr_t naddy = addy + (0x20 * index);
    mem->Write<uint8_t>(naddy + 0x10, 1);
}

uintptr_t GetUnderPaint(const uintptr_t pWeapon)
{
    return mem->Read<uintptr_t>(mem->Read<uintptr_t>(pWeapon + 0xAA0));
}

void UpdatePaint(const uintptr_t& pWeapon)
{
    const CSWeaponType oType = GetWeaponType(pWeapon);
    const uintptr_t oPaintPtr = GetUnderPaint(pWeapon);
    const uintptr_t addy = mem->Read<uintptr_t>(client + 0x1E2A498);//C:\\buildworker\\csgo_rel_win64\\build\\src\\game\\client\\cdll_client_int.cpp
    //4C 8B 05 ? ? ? ? 33 ED 0F B6 F8

    const uint8_t index = GetIndex(addy, pWeapon);
    while (GetUnderPaint(pWeapon) == oPaintPtr)
    {
        //SetWeaponType(pWeapon, CSWeaponType::WEAPONTYPE_UNKNOWN);
        SetUpdateId(addy, index);
        //for (int i = 0; i < 1000; i++)
        //{
        //    
        //}
    }

    SetWeaponType(pWeapon, oType);
}

int main()
{
    const uintptr_t addy = mem->Read<uintptr_t>(client + 0x1E2A498);
    const uintptr_t localPlayer = GetLocalPlayer();
    const uintptr_t activeWeapon = GetActiveWeapon(localPlayer);

    UpdatePaint(activeWeapon);

    return 0;

    //const uintptr_t localPlayer = GetLocalPlayer();
    ////const uintptr_t CGameEntitySystem = mem->Read<uintptr_t>(client + Offsets::dwGameEntitySystem);
    //while (true)
    //{
    //    const uintptr_t activeWeapon = GetActiveWeapon(localPlayer);
    //    //int i = GetClientEnt(CGameEntitySystem, GetClientEntIndex(CGameEntitySystem, activeWeapon)).PostDataUpdateId;
    //    //if(i != 4)
    //    //    std::cout << std::dec << i << std::endl;
    //
    //    const uintptr_t item = activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item;
    //    //
    //    //mem->Write<uint32_t>(activeWeapon + Offsets::m_nFallbackPaintKit, 1);
    //    mem->Write<uint32_t>(activeWeapon + Offsets::m_nFallbackPaintKit, 344);
    //    mem->Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);
    //}
    //
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
        const uintptr_t activeWeapon = GetActiveWeapon(localPlayer);
        const uintptr_t item = activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item;
        CurrentWeaponDef = static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex));
        if (!localPlayer || !activeWeapon || !item || !CurrentWeaponDef)
            continue;

        for (const uintptr_t& weapon : GetWeapons(localPlayer))
        {
            if (!IsMeleeWeapon(weapon))
                OnWeapon(weapon);

            const uintptr_t CGameEntitySystem = mem->Read<uintptr_t>(client + Offsets::dwGameEntitySystem);
            const uint32_t clientWeaponIndex = GetClientEntIndex(CGameEntitySystem, weapon);
            if (!clientWeaponIndex || clientWeaponIndex == 1)
                continue;

            SetPostDataUpdateId(CGameEntitySystem, clientWeaponIndex, PostDataUpdateIds::Update);

            //if (AttributesUpdated(weapon))
            //    UpdateWeapon(weapon);
        }

        ////std::cout << std::hex << weapon + Offsets::m_nFallbackPaintKit << std::endl;
        //continue;      
    }

    overlay::CloseOverlay();

    return 0;
}