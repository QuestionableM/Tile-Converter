#pragma once

extern "C"
{
	#include <lstate.h>
}

namespace SM
{
	namespace Lua
	{
		struct Base
		{
			static int Error(lua_State* L);

			static int Assert(lua_State* L);
			//Loads the contents of lua file into the current environment
			static int Dofile(lua_State* L);

			static int Type(lua_State* L);

			static int Print(lua_State* L);

			static int ToString(lua_State* L);

			//Registers all the lua base functions
			static void Register(lua_State* L);
		};
	}
}