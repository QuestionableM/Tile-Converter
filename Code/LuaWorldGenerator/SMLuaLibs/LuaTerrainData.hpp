#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace TerrainData
		{
			void Register(lua_State* L);
		}
	}
}