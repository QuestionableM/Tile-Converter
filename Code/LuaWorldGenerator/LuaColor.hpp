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

			int ToString(lua_State* L);

			int Mul(lua_State* L);
			int Div(lua_State* L);
			int Add(lua_State* L);
			int Sub(lua_State* L);

			int Equals(lua_State* L);

			int GetR(lua_State* L);
			int GetG(lua_State* L);
			int GetB(lua_State* L);
			int GetA(lua_State* L);

			int SetR(lua_State* L);
			int SetG(lua_State* L);
			int SetB(lua_State* L);
			int SetA(lua_State* L);

			int NewIndex(lua_State* L);
			int Index(lua_State* L);

			void Register(lua_State* L);
		}
	}
}