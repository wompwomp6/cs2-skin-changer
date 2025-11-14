#include "../../../ext/mem.h"

#pragma once

const uintptr_t client = mem->GetModuleBase(L"client.dll");
static const uintptr_t entitylist = mem->Read<uintptr_t>(client + Offsets::dwEntityList);

uintptr_t GetLocalPlayer()
{
    return mem->Read<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
}

uintptr_t GetEntityByHandle(const auto& handle)
{
    if(!handle)
		return NULL;

    const uintptr_t listentry = mem->Read<uintptr_t>(entitylist + 0x8 * ((handle & 0x7FFF) >> 9) + 0x10);

    return mem->Read<uintptr_t>(listentry + 0x70 * (handle & 0x1FF));
}

uint16_t GetEntityHandle(const uintptr_t& ent)
{
    for (uint16_t i = 0; i < 2000; i++)
    {
        if(ent == GetEntityByHandle(i))
			return i;
    }

    return NULL;
}