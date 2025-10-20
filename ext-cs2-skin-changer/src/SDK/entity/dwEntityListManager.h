#include "../../../ext/mem.h"

#pragma once

uintptr_t GetEntityByHandle(const auto& handle)
{
    static const auto entitylist = mem->Read<uintptr_t>(mem->GetModuleBase(L"client.dll") + Offsets::dwEntityList);

    const auto listentry = mem->Read<uintptr_t>(entitylist + 0x8 * ((handle & 0x7FFF) >> 9) + 0x10);

    return mem->Read<uintptr_t>(listentry + 0x70 * (handle & 0x1FF));
}