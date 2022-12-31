#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Util
		{
			int Lerp(lua_State* L);
			int Clamp(lua_State* L);
			int PositiveModulo(lua_State* L);

			void Register(lua_State* L);
		}
	}
}