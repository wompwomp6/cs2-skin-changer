#include "../../../ext/sigs.h"
#include "../../../ext/offsets.h"
#include "../../../ext/wcl.h"

#include "../entity/dwEntityListManager.h"
#include "CHudWeapon.h"
#include "CAttributeList.h"

#pragma once

void SetMeshMask(const uintptr_t& ent, const uint64_t mask)
{
    const auto& node = mem->Read<uintptr_t>(ent + Offsets::m_pGameSceneNode);
    const auto model = node + Offsets::m_modelState;
    const auto dirtyAttributes = mem->Read<uintptr_t>(model + Offsets::m_pDrityModelData);

    for (int i = 0; i < 1000; i++)
    {
        mem->Write<uint64_t>(model + Offsets::m_MeshGroupMask, mask);
        mem->Write<uint64_t>(dirtyAttributes + Offsets::m_DrityMeshGroupMask, mask);
    }
}

//void UpdateWeapon(const uintptr_t& weapon)
//{
//    UpdateHudWeapon(weapon);
//    wcl->CallFunction(Sigs::RegenerateWeaponSkins);
//}
//
//void SetModel(const uintptr_t& weapon, std::string model)
//{
//    const uintptr_t pModel = mem->Allocate(NULL, MemPage);
//    mem->WriteString(pModel, model);
//
//    wcl->CallFunction(Sigs::SetModel,
//        {
//            CArg{ ASM::RCX, weapon },
//            CArg{ ASM::RDX, pModel },
//        });
//
//    mem->Free(pModel, MemPage);
//}
//
//void UpdateModel(const uintptr_t& weapon)
//{
//    SetModel(weapon, "");
//}
//
//void UpdateSubclass(const uintptr_t& weapon)
//{
//    wcl->CallFunction(Sigs::SubclassUpdate,
//    {
//        CArg{ ASM::RCX, weapon },
//    });
//}
//
//namespace Attributes
//{
//    const std::string Skin = "set item texture prefab";
//    const std::string Seed = "set item texture seed";
//    const std::string Wear = "set item texture wear";
//    const std::string StatTrack = "kill eater";
//}
//
//void AddAttribute(const uintptr_t item, const std::string attribute, float value)
//{
//    const uintptr_t pAttribute = mem->Allocate(NULL, MemPage);
//    const uintptr_t pValue = mem->FuncAlloc(Sigs::SetAttribute);
//    mem->WriteString(pAttribute, attribute);
//    mem->Write<float>(pValue, value);
//
//    wcl->CallFunction(Sigs::SetAttribute,
//        {
//            CArg{ ASM::RCX, item },
//            CArg{ ASM::RDX, pAttribute },
//            CArg{ ASM::Xmm2, pValue }
//        }
//    );
//
//    mem->Free(pAttribute, MemPage);
//    mem->Free(pValue, MemPage);
//}

inline uintptr_t GetVdata(const uintptr_t& pWeapon)
{
    return mem->Read<uintptr_t>(pWeapon + Offsets::m_pVdata);
}

inline bool IsMeleeWeapon(const uintptr_t& pWeapon)
{
    return mem->Read<bool>(GetVdata(pWeapon) + Offsets::m_bMeleeWeapon);
}

inline CSWeaponType GetWeaponType(const uintptr_t& pWeapon)
{
    return mem->Read<CSWeaponType>(GetVdata(pWeapon) + Offsets::m_WeaponType);
}

inline void SetWeaponType(const uintptr_t& pWeapon, const CSWeaponType type)
{
    mem->Write<CSWeaponType>(GetVdata(pWeapon) + Offsets::m_WeaponType, type);
}