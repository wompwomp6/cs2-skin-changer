#include <iostream>

#include "mem.h"

#pragma once

namespace Sigs
{
	const auto RegenerateWeaponSkins = mem->SigScan(L"client.dll", "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");
	const auto SetModel = mem->SigScan(L"client.dll", "40 53 48 83 EC 20 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24");
	const auto SetMeshMask = mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71");
	const auto SubclassUpdate = mem->SigScan(L"client.dll", "40 53 48 83 EC 30 48 8B 41 10 48 8B D9 8B 50 30");
	const auto SetAttribute = mem->SigScan(L"client.dll", "40 53 48 83 EC ? 48 8B D9 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 8B 8B");
	const auto UpdateModel = mem->SigScan(L"client.dll", "40 55 53 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 44 0F B6 FA");
	const auto UpdateComposite = mem->SigScan(L"client.dll", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 56 41 57 48 83 EC 20 44 0F B6 F2 48 8B F1 E8");
	const auto UpdateComposite2 = mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 0F B6 F2");
	const auto Regen = mem->SigScan(L"client.dll", "B2 ? E8 ? ? ? ? 33 D2 48 8B CB");
	const auto UpdateSkin = mem->SigScan(L"client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8D 8F ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8B CF E8 ? ? ? ? 48 8B F0 F6 C3 ? 74 ? 33 D2 48 8B CF E8 ? ? ? ? 48 8D 8F ? ? ? ? 48 8B D7 E8 ? ? ? ? 45 33 C0 BA ? ? ? ? 48 8B CF E8 ? ? ? ? 48 85 F6 74 ? 48 8B 06 48 8B CE FF 90 ? ? ? ? 84 C0 75 ? 48 8B CF E8 ? ? ? ? 48 8B 5C 24 ? 48 8B 74 24 ? 48 83 C4 ? 5F C3 CC CC 48 85 D2");
}