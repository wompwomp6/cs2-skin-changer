#include "../item/itemview.h"
#include "../../../ext/WCL.h"
#include "../../../ext/sigs.h"

#pragma once

class CEconEntity
{
private:
    uintptr_t address;

public:
    CEconEntity(uintptr_t pWeapon)
    {
        address = pWeapon;
    }

    uintptr_t GetBase()
    {
        return address;
    }

    bool IsKnife()
    {
		auto item = this->GetItem();
		auto result = item->GetItemDefinition() == 42;
		delete item;

        return result;
    }

    void SetMeshMask(uint64_t mask)
    {
        uintptr_t weaponNode = mem->Read<uintptr_t>(address + Offsets::m_pGameSceneNode);
        std::cout << std::hex << mem->Read<uintptr_t>(weaponNode + 0x108) + 0x10 << std::endl;

        //CParamHandler* handler = new CParamHandler({
        //        CParam(weaponNode, ArgumentType::Ptr),
        //    });
        //
        //wcl->CallFunction(Sigs::SetMeshMask, CallingConventions::FastCall, handler);
    }

    CItemView* GetItem()
    {
        return new CItemView(address + Offsets::m_AttributeManager + Offsets::m_Item);
    }

    void SetFallbackPaintKit(int32_t paint)
    {
        mem->Write<int32_t>(address + Offsets::m_nFallbackPaintKit, paint);
    }

    int32_t GetFallbackPaintKit()
    {
        return mem->Read<int32_t>(address + Offsets::m_nFallbackPaintKit);
    }

    void SetFallbackWear(float wear)
    {
        mem->Write<float>(address + Offsets::m_flFallbackWear, wear);
    }

    float GetFallbackWear()
    {
        return mem->Read<float>(address + Offsets::m_flFallbackWear);
    }

    void SetFallbackStatTrak(int32_t statTrackValue)
    {
        mem->Write<int32_t>(address + Offsets::m_nFallbackStatTrak, statTrackValue);
    }

    int32_t GetFallbackStatTrak()
    {
        return mem->Read<int32_t>(address + Offsets::m_nFallbackStatTrak);
    }

    void SetFallbackSeed(int32_t seed)
    {
        mem->Write<int32_t>(address + Offsets::m_nFallbackSeed, seed);
    }

    void SetAccountID()
    {
        mem->Write<uint32_t>(this->GetItem()->GetItemAddress() + Offsets::m_iAccountID, mem->Read<uint32_t>(address + Offsets::m_OriginalOwnerXuidLow));
    }

    void GetAccountID()
    {
        mem->Read<uint32_t>(this->GetItem()->GetItemAddress() + Offsets::m_iAccountID);
    }
};