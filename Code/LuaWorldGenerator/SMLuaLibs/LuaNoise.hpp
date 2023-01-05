#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Noise
		{
			int PerlinNoise2D(lua_State* L);
			int IntNoise2D(lua_State* L);

			void Register(lua_State* L);
		}
	}
}