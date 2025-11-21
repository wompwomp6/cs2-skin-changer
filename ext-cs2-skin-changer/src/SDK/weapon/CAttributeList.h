#include "../../../ext/mem.h"
#include "../../../ext/skindb.h"

//class CEconItemAttribute
//{
//public:
//	uintptr_t vtable; //0x0000
//	uintptr_t m_pWeapon; //0x0008
//	char pad_0010[24]; //0x0010
//	int32_t m_hHandle; //0x0028
//	char pad_002C[4]; //0x002C
//	uint16_t m_iAttributeDefinitionIndex; //0x0030
//	char pad_0032[2]; //0x0032
//	float m_flValue; //0x0034
//	float m_flInitialValue; //0x0038
//	int32_t m_nRefundableCurrency; //0x003C
//	bool m_bSetBonus; //0x0040
//	char pad_0041[7]; //0x0041
//}; //Size: 0x0048

enum AttributeDefinitionIndex : uint16_t
{
	Paint = 6,
	Pattern = 7,
	Wear = 8,
	StatTrack = 80,
	NameTag = 81,//not sure how to use this one
};

class CEconItemAttribute
{
public:
	const uintptr_t vtable; //0x0000
	const uintptr_t pOwner; //0x0008
	const char pad_0010[32]; //0x0010
	const AttributeDefinitionIndex attributeDefinitionIndex; //0x0030
	const char pad_0032[2]; //0x0032
	const float value; //0x0034
	const float initValue; //0x0038
	const char pad_003C[4]; //0x003C
	const bool setBonus; //0x0040
	const char pad_0041[7]; //0x0041
}; //Size: 0x0048

class CAttribute
{
public:
	const uint64_t uSize; //0x0000
	const uintptr_t pAttributes; //0x0008
}; //Size: 0x0010

inline CEconItemAttribute GetAttributeByIndex(const uintptr_t pAttributeArray, const uint16_t index)
{
	const uintptr_t econItemAttribute = pAttributeArray + (sizeof(CEconItemAttribute) * index);
	return mem->Read<CEconItemAttribute>(econItemAttribute);
}

std::vector<CEconItemAttribute> GetAttributes(const uintptr_t pItem)
{
	const CAttribute attribute = mem->Read<CAttribute>(pItem + Offsets::m_AttributeList + Offsets::m_Attributes);
	if(!attribute.pAttributes)
		return {};

	std::vector<CEconItemAttribute> attributes;
	for (uint16_t i = 0; i < attribute.uSize; i++)
		attributes.push_back(GetAttributeByIndex(attribute.pAttributes, i));

	return attributes;
}

bool AttributesUpdated(const uintptr_t pWeapon)
{
	const uintptr_t item = pWeapon + Offsets::m_AttributeManager + Offsets::m_Item;
	SkinInfo_t Skin = skinManager->GetSkin(static_cast<WeaponsEnum>(mem->Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex)));
	std::vector<CEconItemAttribute> attributes = GetAttributes(item);
	if (!Skin.Paint || attributes.empty())
		return false;

	for (const CEconItemAttribute attribute : attributes)
	{
		if (attribute.attributeDefinitionIndex == AttributeDefinitionIndex::Paint)
			return mem->Read<int32_t>(pWeapon + Offsets::m_nFallbackPaintKit) == Skin.Paint;
	}

	return false;
}