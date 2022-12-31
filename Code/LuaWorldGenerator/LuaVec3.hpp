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
		namespace Vec3
		{
			glm::vec3* CreateVector3(lua_State* L);

			int New(lua_State* L);
			int Zero(lua_State* L);
			int One(lua_State* L);

			int Equals(lua_State* L);

			int Mul(lua_State* L);
			int Div(lua_State* L);
			int Add(lua_State* L);
			int Sub(lua_State* L);

			int Length(lua_State* L);
			int Length2(lua_State* L);
			int Normalize(lua_State* L);
			int SafeNormalize(lua_State* L);
			int Cross(lua_State* L);
			int Lerp(lua_State* L);
			int Dot(lua_State* L);
			int Min(lua_State* L);
			int Max(lua_State* L);
			int GetRotation(lua_State* L);

			int Rotate(lua_State* L);
			int RotateX(lua_State* L);
			int RotateY(lua_State* L);
			int RotateZ(lua_State* L);

			int GetX(lua_State* L);
			int GetY(lua_State* L);
			int GetZ(lua_State* L);

			int SetX(lua_State* L);
			int SetY(lua_State* L);
			int SetZ(lua_State* L);

			int ToString(lua_State* L);

			int NewIndex(lua_State* L);
			int Index(lua_State* L);

			void Register(lua_State* L);
		};
	}
}