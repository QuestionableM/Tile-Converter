#pragma once

#include "Utils/Json.hpp"
#include "ObjectDatabase/ObjectData.hpp"

class DatabaseLoader
{
	static void LoadGameDatabase();
	static void LoadModDatabase();

public:
	static void LoadDatabase();
};