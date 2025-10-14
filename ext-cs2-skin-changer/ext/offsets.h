#include <iostream>

#pragma once

namespace Offsets
{
    constexpr std::ptrdiff_t dwEntityList = 0x1D16758;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1BF1FA0;

    ///////////////////////////////////////////////////////////

    constexpr std::ptrdiff_t m_pWeaponServices = 0x1408; // CPlayer_WeaponServices*
    constexpr std::ptrdiff_t m_hActiveWeapon = 0x58; // CHandle<C_BasePlayerWeapon>

    constexpr std::ptrdiff_t m_hHudModelArms = 0x243C; // CHandle<C_CS2HudModelArms>
    constexpr std::ptrdiff_t m_hOwnerEntity = 0x520; // CHandle<C_BaseEntity>

    constexpr std::ptrdiff_t m_pGameSceneNode = 0x330; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pChild = 0x40; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pNextSibling = 0x48; // CGameSceneNode*
    constexpr std::ptrdiff_t m_pOwner = 0x30; // CEntityInstance*

    constexpr std::ptrdiff_t m_modelState = 0x190; // CModelState
    constexpr std::ptrdiff_t m_MeshGroupMask = 0x250; // uint64

    constexpr std::ptrdiff_t m_nFallbackPaintKit = 0x1888; // int32
    constexpr std::ptrdiff_t m_nFallbackSeed = 0x188C; // int32
    constexpr std::ptrdiff_t m_flFallbackWear = 0x1890; // float32
    constexpr std::ptrdiff_t m_nFallbackStatTrak = 0x1894; // int32

    constexpr std::ptrdiff_t m_OriginalOwnerXuidLow = 0x1880; // uint32

    constexpr std::ptrdiff_t m_AttributeManager = 0x13A8; // C_AttributeContainer
    constexpr std::ptrdiff_t m_Item = 0x50; // C_EconItemView

    constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA; // uint16
    constexpr std::ptrdiff_t m_iItemIDHigh = 0x1D0; // uint32
    constexpr std::ptrdiff_t m_iAccountID = 0x1D8; // uint32
    constexpr std::ptrdiff_t m_szCustomNameOverride = 0x3A1; // char[161]
}