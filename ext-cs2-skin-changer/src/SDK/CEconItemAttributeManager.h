#include "../../ext/mem.h"
#include "../../ext/sigs.h"

class CEconItemAttribute
{
public:
	uintptr_t vtable; //0x0000
	uintptr_t m_pWeapon; //0x0008
	char pad_0010[24]; //0x0010
	int32_t m_hHandle; //0x0028
	char pad_002C[4]; //0x002C
	uint16_t m_iAttributeDefinitionIndex; //0x0030
	char pad_0032[2]; //0x0032
	float m_flValue; //0x0034
	float m_flInitialValue; //0x0038
	int32_t m_nRefundableCurrency; //0x003C
	bool m_bSetBonus; //0x0040
	char pad_0041[7]; //0x0041
}; //Size: 0x0048

enum AttributeDefinitionIndex
{
	Paint = 6,
	Pattern = 7,
	Wear = 7,
	StatTrack = 80,
	NameTag = 81,//not sure how to use this one
};

class CEconItemAttributeManager
{
private:
	const uintptr_t memPage = mem->Allocate(NULL, 1);
public:
	CEconItemAttribute MakeItemAttribute(uintptr_t pWeapon, AttributeDefinitionIndex definition, float value)
	{
		CEconItemAttribute itemAttribute;
		itemAttribute.vtable = Sigs::CEconItemAttributeVtable;
		itemAttribute.m_pWeapon = pWeapon;
		itemAttribute.m_hHandle = -1;
		itemAttribute.m_iAttributeDefinitionIndex = definition;
		itemAttribute.m_flValue = value;
		itemAttribute.m_flInitialValue = value;

		return itemAttribute;
	}

	bool IsValid(const CEconItemAttribute& attribute)
	{
		return attribute.m_flValue && attribute.m_flInitialValue && attribute.m_iAttributeDefinitionIndex 
				      && attribute.m_pWeapon && attribute.vtable == Sigs::CEconItemAttributeVtable;
	}

	CEconItemAttribute GetItemAttribute(uint8_t index)
	{
		if (!index || index > MemPage / sizeof(CEconItemAttribute))
			return CEconItemAttribute();

		return mem->Read<CEconItemAttribute>(memPage + (sizeof(CEconItemAttribute) * index));
	}

	CEconItemAttribute SetItemAttribute(uint8_t index)
	{
		if (!index || index > MemPage / sizeof(CEconItemAttribute))
			return CEconItemAttribute();

		return mem->Read<CEconItemAttribute>(memPage + (sizeof(CEconItemAttribute) * index));
	}
};