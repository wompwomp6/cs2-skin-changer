#pragma once

#include "../../../ext/mem.h"

const uintptr_t client = mem.GetModuleBase(L"client.dll");
static const uintptr_t entitylist = mem.Read<uintptr_t>(client + Offsets::dwEntityList);

#define ChunkSize 512
#define ChunkArraySize 3
#define MaxEntities ChunkArraySize * ChunkSize

#define EntityIdentitySize 0x70

uintptr_t GetLocalPlayer()
{
    return mem.Read<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
}

uintptr_t GetLocalController()
{
    return mem.Read<uintptr_t>(client + Offsets::dwLocalPlayerController);
}

inline uintptr_t GetEntityByHandle(const auto& handle)
{
    const uintptr_t listentry = mem.Read<uintptr_t>(entitylist + 0x8 * ((handle & 0x7FFF) >> 9) + 0x10);
    return mem.Read<uintptr_t>(listentry + 0x70 * (handle & 0x1FF));
}

uint16_t GetEntityHandle(const uintptr_t ent)
{
    const uintptr_t indentity = mem.Read<uintptr_t>(ent);

	return mem.Read<uint16_t>(indentity + 0x10);
}