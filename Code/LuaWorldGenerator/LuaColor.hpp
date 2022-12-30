#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Color
		{
			int New(lua_State* L);

			void Register(lua_State* L);
		}
	}
}