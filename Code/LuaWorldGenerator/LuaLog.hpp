#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Log
		{
			int Info(lua_State* L);
			int Warning(lua_State* L);
			int Error(lua_State* L);

			void Register(lua_State* L);
		}
	}
}