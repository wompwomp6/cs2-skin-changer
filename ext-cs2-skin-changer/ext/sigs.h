#include <iostream>

#include "mem.h"

#pragma once

namespace Sigs
{
	const auto RegenerateWeaponSkins = mem->SigScan(L"client.dll", "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");
	const auto SwitchHands = mem->SigScan(L"client.dll", "40 53 48 83 EC ? 33 C9 E8 ? ? ? ? 48 8B D8 48 85 C0 74 ? 48 8B 00 48 8B CB FF 90 ? ? ? ? 84 C0 74 ? 48 8B 03 48 8B CB FF 90 ? ? ? ? 84 C0 74 ? 0F B6 83");

	const auto SetFallBackDataPatch = mem->SigScan(L"client.dll", "F6 C3 ? 0F 84 ? ? ? ? 48 8B 87 ? ? ? ? 48 8D 8F");
}