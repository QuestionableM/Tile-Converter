#pragma once

#include "ObjectDatabase\Mod\Mod.hpp"

#include <string>

class DecalsetReader
{
public:
	static void LoadFromFile(const std::wstring& path, Mod* mod);
};