#include "a2x/client_dll.hpp"
#include "a2x/offsets.hpp"

#pragma once

namespace Offsets
{
    constexpr std::ptrdiff_t dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;

    ///////////////////////////////////////////////////////////

    constexpr std::ptrdiff_t m_pClippingWeapon = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_pClippingWeapon; // C_CSWeaponBase*
    constexpr std::ptrdiff_t m_hHudModelArms = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_hHudModelArms; // CHandle<C_CS2HudModelArms>
    constexpr std::ptrdiff_t m_hOwnerEntity = cs2_dumper::schemas::client_dll::C_BaseEntity::m_hOwnerEntity; // CHandle<C_BaseEntity>

    constexpr std::ptrdiff_t m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pChild = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pChild; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pNextSibling = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pNextSibling; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pOwner = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pOwner; // CEntityInstance*

    constexpr std::ptrdiff_t m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState; // CModelState
    constexpr std::ptrdiff_t m_MeshGroupMask = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask; // uint64

    constexpr std::ptrdiff_t m_nFallbackPaintKit = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit; // int32
    constexpr std::ptrdiff_t m_nFallbackSeed = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed; // int32
    constexpr std::ptrdiff_t m_flFallbackWear = cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear; // float32
    constexpr std::ptrdiff_t m_nFallbackStatTrak = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak; // int32
    constexpr std::ptrdiff_t m_OriginalOwnerXuidLow = cs2_dumper::schemas::client_dll::C_EconEntity::m_OriginalOwnerXuidLow; // uint32

    constexpr std::ptrdiff_t m_AttributeManager = cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager; // C_AttributeContainer
    constexpr std::ptrdiff_t m_Item = cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item; // C_EconItemView

    constexpr std::ptrdiff_t m_iItemDefinitionIndex = cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex; // uint16
    constexpr std::ptrdiff_t m_iItemIDHigh = cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh; // uint32
    constexpr std::ptrdiff_t m_iAccountID = cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID; // uint32
    constexpr std::ptrdiff_t m_szCustomNameOverride = cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomNameOverride; // char[161]
}