#include "entity.h"
#include "../../../ext/offsets.h"

#pragma once

uintptr_t GetHudWeapon(const uintptr_t localPlayer, const uintptr_t weapon)
{
    const auto& armsBase = GetEntityByHandle(mem->Read<uint32_t>(localPlayer + Offsets::m_hHudModelArms));
    const auto& armsNode = mem->Read<uintptr_t>(armsBase + Offsets::m_pGameSceneNode);
    for (uintptr_t viewModel = mem->Read<uintptr_t>(armsNode + Offsets::m_pChild); viewModel; viewModel = mem->Read<uintptr_t>(viewModel + Offsets::m_pNextSibling))
    {
        if (!viewModel || !mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner))
            continue;

        if (GetEntityByHandle(mem->Read<uint32_t>(mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner) + Offsets::m_hOwnerEntity)) != weapon)
            continue;

        return mem->Read<uintptr_t>(viewModel + Offsets::m_pOwner);
    }

    return NULL;
}