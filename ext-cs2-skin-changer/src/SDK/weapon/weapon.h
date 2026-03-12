#include <iostream>

#pragma once

#define HideHudFlag 128
#define STRINGTOKEN_MURMURHASH_SEED 0x31415926
#define ForcePaintKitRegen CSWeaponType::WEAPONTYPE_UNKNOWN
enum PostDataUpdateIds
{
    None = 0,
    Update = 1,
    Setup = 2,
    Idle = 4,
};
enum ItemIds
{
    NoSkinValues = 0,
    UseFallBackValues = -1,
}; 

enum WeaponsEnum : uint16_t
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
    CtKnife = 42,          // kept
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
    Cz75A = 63,
    Revolver = 64,
    Tknife = 59,           // kept
    Galil = 13,
    Mp5SD = 23,

    // ===== added (map weapons, non-knife) =====
    Shield = 37,
    Healthshot = 57,
    TagGrenade = 68,
    Fists = 69,
    BreachCharge = 70,
    Tablet = 72,
    Melee = 74,
    Axe = 75,
    Hammer = 76,
    Spanner = 78,
    Snowball = 84,
    BumpMine = 85
};

enum class CSWeaponType : uint32_t
{
    WEAPONTYPE_KNIFE = 0x0,
    WEAPONTYPE_PISTOL = 0x1,
    WEAPONTYPE_SUBMACHINEGUN = 0x2,
    WEAPONTYPE_RIFLE = 0x3,
    WEAPONTYPE_SHOTGUN = 0x4,
    WEAPONTYPE_SNIPER_RIFLE = 0x5,
    WEAPONTYPE_MACHINEGUN = 0x6,
    WEAPONTYPE_C4 = 0x7,
    WEAPONTYPE_TASER = 0x8,
    WEAPONTYPE_GRENADE = 0x9,
    WEAPONTYPE_EQUIPMENT = 0xA,
    WEAPONTYPE_STACKABLEITEM = 0xB,
    WEAPONTYPE_UNKNOWN = 0xC
};
//server.dll ^^^
CSWeaponType WeaponEnumToType(WeaponsEnum weapon)
{
    switch (weapon)
    {
        // --- KNIVES ---
    case CtKnife:
    case Tknife:
        return CSWeaponType::WEAPONTYPE_KNIFE;

        // --- PISTOLS ---
    case Deagle:
    case Elite:
    case FiveSeven:
    case Glock:
    case Tec9:
    case P200:
    case p250:
    case UspS:
    case Cz75A:
    case Revolver:
        return CSWeaponType::WEAPONTYPE_PISTOL;

        // --- SMGS ---
    case Mac10:
    case Mp7:
    case Mp9:
    case P90:
    case Bizon:
    case Ump45:
    case Mp5SD:
        return CSWeaponType::WEAPONTYPE_SUBMACHINEGUN;

        // --- RIFLES ---
    case Ak47:
    case Famas:
    case Galil:
    case Aug:
    case Sg556:
    case M4A4:
    case M4A1Silencer:
        return CSWeaponType::WEAPONTYPE_RIFLE;

        // --- SNIPERS ---
    case Awp:
    case Ssg08:
    case Scar20:
    case G3Sg1:
        return CSWeaponType::WEAPONTYPE_SNIPER_RIFLE;

        // --- SHOTGUNS ---
    case Nova:
    case Xm1014:
    case Mag7:
    case Sawedoof:
        return CSWeaponType::WEAPONTYPE_SHOTGUN;

        // --- MACHINEGUNS ---
    case M249:
    case Negev:
        return CSWeaponType::WEAPONTYPE_MACHINEGUN;

        // --- GRENADES ---
    case FlashBang:
    case HeGrenade:
    case SmokeGrenade:
    case Molotov:
    case IncGrenade:
    case Decoy:
        return CSWeaponType::WEAPONTYPE_GRENADE;

        // --- C4 ---
    case C4:
        return CSWeaponType::WEAPONTYPE_C4;

        // --- ZEUS (equipment) ---
    case Zeus:
        return CSWeaponType::WEAPONTYPE_TASER;

        // --- NONE / default ---
    case none:
    default:
        return CSWeaponType::WEAPONTYPE_UNKNOWN; // or UNKNOWN if you add one later
    }
}
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