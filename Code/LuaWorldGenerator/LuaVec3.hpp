#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Vec3
		{
			int New(lua_State* L);

			int Zero(lua_State* L);

			int One(lua_State* L);

			int Equals(lua_State* L);

			int Normalize(lua_State* L);

			int Cross(lua_State* L);

			int GetX(lua_State* L);
			int GetY(lua_State* L);
			int GetZ(lua_State* L);

			int SetX(lua_State* L);
			int SetY(lua_State* L);
			int SetZ(lua_State* L);

			int NewIndex(lua_State* L);

			int Index(lua_State* L);

			void Register(lua_State* L);
		};
	}
}