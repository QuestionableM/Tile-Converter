#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

#include <glm.hpp>

namespace SM
{
	namespace Lua
	{
		namespace Quat
		{
			glm::quat* CreateQuaternion(lua_State* L);

			int New(lua_State* L);
			int Identity(lua_State* L);

			int GetX(lua_State* L);
			int GetY(lua_State* L);
			int GetZ(lua_State* L);
			int GetW(lua_State* L);

			int SetX(lua_State* L);
			int SetY(lua_State* L);
			int SetZ(lua_State* L);
			int SetW(lua_State* L);

			int GetUp(lua_State* L);
			int GetRight(lua_State* L);
			int GetAt(lua_State* L);

			int AngleAxis(lua_State* L);
			int Inverse(lua_State* L);
			int FromEuler(lua_State* L);
			int Slerp(lua_State* L);
			int LookRotation(lua_State* L);

			void Register(lua_State* L);
		}
	}
}