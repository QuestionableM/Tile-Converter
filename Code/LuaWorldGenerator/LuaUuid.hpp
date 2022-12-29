#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Uuid
		{
			int New(lua_State* L);

			int GetNil(lua_State* L);

			int GenerateNamed(lua_State* L);

			int GenerateRandom(lua_State* L);

			int ToString(lua_State* L);

			int Equals(lua_State* L);

			int IsNil(lua_State* L);

			void Register(lua_State* L);
		}
	}
}