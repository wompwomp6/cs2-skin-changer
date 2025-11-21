#include "a2x/client_dll.hpp"
#include "a2x/offsets.hpp"

#pragma once

namespace Offsets
{
    constexpr std::ptrdiff_t dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
    constexpr std::ptrdiff_t dwGameEntitySystem = cs2_dumper::offsets::client_dll::dwGameEntitySystem;
    constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = cs2_dumper::offsets::client_dll::dwGameEntitySystem_highestEntityIndex;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;

    ///////////////////////////////////////////////////////////

    constexpr std::ptrdiff_t m_pClippingWeapon = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_pClippingWeapon; // C_CSWeaponBase*
    constexpr std::ptrdiff_t m_pWeaponServices = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices; // CPlayer_WeaponServices*
    constexpr std::ptrdiff_t m_hHudModelArms = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_hHudModelArms; // CHandle<C_CS2HudModelArms>
    constexpr std::ptrdiff_t m_hOwnerEntity = cs2_dumper::schemas::client_dll::C_BaseEntity::m_hOwnerEntity; // CHandle<C_BaseEntity>
    constexpr std::ptrdiff_t m_pEntity = cs2_dumper::schemas::client_dll::CEntityInstance::m_pEntity; // CEntityIdentity*
    constexpr std::ptrdiff_t m_flags = cs2_dumper::schemas::client_dll::CEntityIdentity::m_flags; // uint32

    constexpr std::ptrdiff_t m_hMyWeapons = 0x40; // C_NetworkUtlVectorBase<CHandle<C_BasePlayerWeapon>>
    constexpr std::ptrdiff_t m_hActiveWeapon = 0x58; // CHandle<C_BasePlayerWeapon>

    constexpr std::ptrdiff_t m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pChild = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pChild; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pNextSibling = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pNextSibling; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pOwner = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pOwner; // CEntityInstance*

    constexpr std::ptrdiff_t m_bNeedToReApplyGloves = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves; // bool
    constexpr std::ptrdiff_t m_EconGloves = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves; // C_EconItemView
    constexpr std::ptrdiff_t m_bInitialized = cs2_dumper::schemas::client_dll::C_EconItemView::m_bInitialized; // bool
    constexpr std::ptrdiff_t m_bRestoreCustomMaterialAfterPrecache = cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache; // bool

    constexpr std::ptrdiff_t m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState; // CModelState
    constexpr std::ptrdiff_t m_MeshGroupMask = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask; // uint64

    constexpr std::ptrdiff_t m_nSubclassID = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID; // uint64
    constexpr std::ptrdiff_t m_bMeleeWeapon = cs2_dumper::schemas::client_dll::CCSWeaponBaseVData::m_bMeleeWeapon; // uint64
    constexpr std::ptrdiff_t m_WeaponType = cs2_dumper::schemas::client_dll::CCSWeaponBaseVData::m_WeaponType; // CSWeaponType

    constexpr std::ptrdiff_t m_nFallbackPaintKit = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit; // uint32
    constexpr std::ptrdiff_t m_nFallbackStatTrak = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak; // uint32
    constexpr std::ptrdiff_t m_flFallbackWear = cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear; // float32
    constexpr std::ptrdiff_t m_nFallbackSeed = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed; // uint32
    constexpr std::ptrdiff_t m_OriginalOwnerXuidLow = cs2_dumper::schemas::client_dll::C_EconEntity::m_OriginalOwnerXuidLow; // uint32

    constexpr std::ptrdiff_t m_AttributeManager = cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager; // C_AttributeContainer
    constexpr std::ptrdiff_t m_Item = cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item; // C_EconItemView

    constexpr std::ptrdiff_t m_AttributeList = cs2_dumper::schemas::client_dll::C_EconItemView::m_AttributeList; // CAttributeList
    constexpr std::ptrdiff_t m_Attributes = cs2_dumper::schemas::client_dll::CAttributeList::m_Attributes; // C_UtlVectorEmbeddedNetworkVar<CEconItemAttribute>
    constexpr std::ptrdiff_t m_iItemDefinitionIndex = cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex; // uint16
    constexpr std::ptrdiff_t m_iAccountID = cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID; // uint32
    constexpr std::ptrdiff_t m_szCustomNameOverride = cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomNameOverride; // char[161]
    constexpr std::ptrdiff_t m_iItemIDHigh = cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh; // uint32

    constexpr std::ptrdiff_t m_pVdata = 0x388;
    constexpr std::ptrdiff_t m_pDrityModelData = 0x108;
    constexpr std::ptrdiff_t m_DrityMeshGroupMask = 0x10;
    constexpr std::ptrdiff_t m_uClientEntCount = 0xC28;
    constexpr std::ptrdiff_t m_pClientEntList = 0xC30;
    constexpr std::ptrdiff_t m_hViewModelWeapon = 0x1950;
}