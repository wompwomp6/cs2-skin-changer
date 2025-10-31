#include "../../../ext/sigs.h"
#include "../../../ext/offsets.h"

#include "../../../ext/wcl.h"

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
    CtKnife = 42,
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
    Tknife = 59,
    Galil = 13,
    Mp5SD = 23
};

std::string WeaponIdToString(int weaponId)
{
    switch (weaponId)
    {
    case 0: return "None";
    case 1: return "Deagle";
    case 2: return "Dual";
    case 3: return "FiveSeven";
    case 4: return "Glock";
    case 7: return "Ak47";
    case 8: return "Aug";
    case 9: return "Awp";
    case 10: return "Famas";
    case 11: return "G3Sg1";
    case 14: return "M249";
    case 17: return "Mac10";
    case 19: return "P90";
    case 24: return "Ump45";
    case 25: return "Xm1014";
    case 26: return "Bizon";
    case 27: return "Mag7";
    case 28: return "Negev";
    case 29: return "Sawedoff";
    case 30: return "Tec9";
    case 31: return "Zeus";
    case 32: return "P200";
    case 33: return "Mp7";
    case 34: return "Mp9";
    case 35: return "Nova";
    case 36: return "p250";
    case 38: return "Scar20";
    case 39: return "Sg556";
    case 40: return "Ssg08";
    case 43: return "FlashBang";
    case 44: return "HeGrenade";
    case 45: return "SmokeGrenade";
    case 46: return "Molotov";
    case 47: return "Decoy";
    case 48: return "IncGrenade";
    case 49: return "C4";
    case 16: return "M4A4";
    case 61: return "Usp";
    case 60: return "M4A1-S";
    case 63: return "Cz";
    case 64: return "R8";
    case 13: return "Galil";
    case 23: return "MP5 SD";
    default: return "None";
    }
}

void UpdateSkin(const uintptr_t weapon)
{
    //wcl->CallFunction(Sigs::RegenerateWeaponSkins);

    

    //wcl->CallFunction(Sigs::UpdateComposite2,
    //    {
    //        CArg{ ASM::RCX, weapon + 0x5F8 },
    //        CArg{ ASM::dl, true },
    //    }
    //    );

    //if (!mem->Read<uint64_t>(weapon + 0xA98))
    //{
    //    wcl->CallFunction(Sigs::UpdateModel,
    //        {
    //            CArg{ ASM::RCX, weapon },
    //            CArg{ ASM::dl, false }
    //        }
    //    );
    //
    //    Sleep(50);
    //}
    //
    wcl->CallFunction(Sigs::UpdateSkin,
        {
            CArg{ ASM::RCX, weapon },
            CArg{ ASM::dl, true },
        }
    );
    
    //mem->SwapPatch(Sigs::ForceUpdateHud, 2, 250);
}

void SetMeshMask(const uintptr_t ent, const uint64_t mask)
{
    wcl->CallFunction(Sigs::SetMeshMask,
        {
            CArg{ ASM::RCX, mem->Read<uintptr_t>(ent + Offsets::m_pGameSceneNode) },
            CArg{ ASM::RDX, mask },
        });
}

namespace Attributes
{
    const std::string Skin = "set item texture prefab";
    const std::string Seed = "set item texture seed";
    const std::string Wear = "set item texture wear";
    const std::string StatTrack = "kill eater";
}

void AddAttribute(const uintptr_t item, const std::string attribute, float value)
{
    const uintptr_t pAttribute = mem->Allocate(NULL, MemPage);
    const uintptr_t pValue = wcl->FuncAlloc(Sigs::SetAttribute);
    mem->WriteString(pAttribute, attribute);
    mem->Write<float>(pValue, value);

    wcl->CallFunction(Sigs::SetAttribute,
        {
            CArg{ ASM::RCX, item },
            CArg{ ASM::RDX, pAttribute },
            CArg{ ASM::Xmm2, pValue }
        }
    );

    mem->Free(pAttribute, MemPage);
    mem->Free(pValue, MemPage);
}