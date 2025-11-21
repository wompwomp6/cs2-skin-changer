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

class ClientEnt
{
public:
    const uintptr_t pCEntityIdentity; //0x0000
    const uint32_t PostDataUpdateId; //0x0008
    //const char pad_000C[4]; //0x000C
    const uint32_t pad_000C; //0x000C
}; //Size: 0x0010

uint64_t GetClientEntCount(const uintptr_t CGameEntitySystem)
{
    uint64_t ClientEntCount = 0;
    while (!ClientEntCount)
    {
        ClientEntCount = mem->Read<uint64_t>(CGameEntitySystem + Offsets::m_uClientEntCount);
    }
    return ClientEntCount;
}

ClientEnt GetClientEnt(const uintptr_t CGameEntitySystem, const uint32_t index)
{
    const uintptr_t clientEntList = mem->Read<uintptr_t>(CGameEntitySystem + Offsets::m_pClientEntList);

    return mem->Read<ClientEnt>(clientEntList + (index * sizeof(ClientEnt)));
}

uint32_t GetClientEntIndex(const uintptr_t CGameEntitySystem, const uintptr_t ent)
{
    const uint64_t clientEntities = GetClientEntCount(CGameEntitySystem);
    for (uint32_t i = 0; i < clientEntities; i++)
    {
        const ClientEnt CurrentEnt = GetClientEnt(CGameEntitySystem, i);
        if (!CurrentEnt.pCEntityIdentity)
            continue;

        if (mem->Read<uintptr_t>(CurrentEnt.pCEntityIdentity) == ent)
            return i;
    }

    return NULL;
}

void SetPostDataUpdateId(const uintptr_t CGameEntitySystem, const uint32_t index, uint32_t newPostDataUpdateId)
{
    const uintptr_t clientEntList = mem->Read<uintptr_t>(CGameEntitySystem + Offsets::m_pClientEntList);

    mem->Write<uint32_t>(clientEntList + (index * sizeof(ClientEnt)) + offsetof(ClientEnt, PostDataUpdateId), newPostDataUpdateId);
    //mem->Write<uint32_t>(clientEntList + (index * sizeof(ClientEnt)) + offsetof(ClientEnt, pad_000C), 3212836864);
}