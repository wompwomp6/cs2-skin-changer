#include "../../../ext/sigs.h"
#include "../../../ext/offsets.h"

#pragma once

enum ItemIds
{
    NoSkinValues = 0,
    UseFallBackValues = -1,
};
enum WeaponsEnum
{
	none = 0,
    Deagle = 1,
    Elite = 2,
    FiveSeven = 3,
    Glock = 4,
    Ak47 = 7,
    Aug = 8,
    Awp = 9,
    Famas = 10,
    G3Sg1 = 11,
    M249 = 14,
    Mac10 = 17,
    P90 = 19,
    Ump45 = 24,
    Xm1014 = 25,
    Bizon = 26,
    Mag7 = 27,
    Negev = 28,
    Sawedoof = 29,
    Tec9 = 30,
    Zeus = 31,
    P200 = 32,
    Mp7 = 33,
    Mp9 = 34,
    Nova = 35,
    p250 = 36,
    Scar20 = 38,
    Sg556 = 39,
    Ssg08 = 40,
    //CtKnife = 42,
    FlashBang = 43,
    HeGrenade = 44,
    SmokeGrenade = 45,
    Molotov = 46,
    Decoy = 47,
    IncGrenade = 48,
    C4 = 49,
    M4A4 = 16,
    UspS = 61,
    M4A1Silencer = 60,
    Cz65A = 63,
    Revolver = 64,
    //Tknife = 59
};

void UpdateSkin()
{
    if (Sigs::SwitchHands)
    {
        mem->CallThread(Sigs::SwitchHands);
        Sleep(10);
        mem->CallThread(Sigs::SwitchHands);
    }

    Sleep(50);

    if (Sigs::RegenerateWeaponSkins)
    {
        mem->CallThread(Sigs::RegenerateWeaponSkins);
    }
}

void SetMeshMask(const uintptr_t ent, const uint64_t mask)
{
    const auto& node = mem->Read<uintptr_t>(ent + Offsets::m_pGameSceneNode);
    const auto model = node + Offsets::m_modelState;
    const auto dirtyAttributes = mem->Read<uintptr_t>(model + 0x108);

    if (model + Offsets::m_MeshGroupMask == mask)
        return;

    for (int i = 0; i < 1000; i++)
    {
        mem->Write<uint64_t>(model + Offsets::m_MeshGroupMask, mask);
    }
    mem->Write<uint64_t>(dirtyAttributes + 0x10, mask);
}