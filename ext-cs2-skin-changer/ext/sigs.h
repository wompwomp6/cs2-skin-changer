#pragma once

#include <iostream>

#include "mem.h"

namespace Sigs
{
	const auto RegenerateWeaponSkins = mem.SigScan(L"client.dll", "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");
}