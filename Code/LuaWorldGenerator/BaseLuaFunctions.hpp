#pragma once

extern "C"
{
	#include <lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Base
		{
			int Error(lua_State* L);

			int Assert(lua_State* L);
			//Loads the contents of lua file into the current environment
			int Dofile(lua_State* L);

			int Type(lua_State* L);

			int Print(lua_State* L);

			int ToString(lua_State* L);

			//Registers all the lua base functions
			void Register(lua_State* L);
		}
	}
}