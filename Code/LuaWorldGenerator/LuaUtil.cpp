#include "LuaUtil.hpp"

#include "BaseLuaFunctions.hpp"
#include "CLuaTableUtils.hpp"

#include <algorithm>
#include <cmath>

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

#include <glm.hpp>

namespace SM
{
	namespace Lua
	{
		int Util::Lerp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //first val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //second val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //interpolation step

			const double v_first_val = lua_tonumber(L, 1);
			const double v_second_val = lua_tonumber(L, 2);
			const double v_step = lua_tonumber(L, 3);

			lua_pushnumber(L, Lua::Math::Lerp(v_first_val, v_second_val, v_step));
			return 1;
		}

		int Util::Clamp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //value
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //min
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //max

			const double v_value = lua_tonumber(L, 1);
			const double v_min = lua_tonumber(L, 2);
			const double v_max = lua_tonumber(L, 3);

			lua_pushnumber(L, std::clamp(v_value, v_min, v_max));
			return 1;
		}

		int Util::PositiveModulo(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			const long long v_int1 = static_cast<long long>(lua_tonumber(L, 1));
			const long long v_int2 = static_cast<long long>(lua_tonumber(L, 2));

			if (v_int2 == 0)
			{
				lua_pushinteger(L, 0);
				return 1;
			}

			lua_pushinteger(L, v_int1 % v_int2);
			return 1;
		}

		int Util::Smoothstep(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			const double v_edge0 = lua_tonumber(L, 1);
			const double v_edge1 = lua_tonumber(L, 2);
			const double v_x = lua_tonumber(L, 3);

			lua_pushnumber(L, glm::smoothstep(v_edge0, v_edge1, v_x));
			return 1;
		}

		int Util::Smootherstep(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			const double v_edge0 = lua_tonumber(L, 1);
			const double v_edge1 = lua_tonumber(L, 2);
			const double v_x = lua_tonumber(L, 3);

			lua_pushnumber(L, Lua::Math::Smootherstep(v_edge0, v_edge1, v_x));
			return 1;
		}

		int Util::Bezier2(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 4);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);

			const double v_c0 = lua_tonumber(L, 1);
			const double v_c1 = lua_tonumber(L, 2);
			const double v_c2 = lua_tonumber(L, 3);
			const double v_t = lua_tonumber(L, 4);

			lua_pushnumber(L, Lua::Math::QuadraticBezier(v_c0, v_c1, v_c2, v_t));
			return 1;
		}

		int Util::Bezier3(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 5);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 5, LUA_TNUMBER);

			const double v_c0 = lua_tonumber(L, 1);
			const double v_c1 = lua_tonumber(L, 2);
			const double v_c2 = lua_tonumber(L, 3);
			const double v_c3 = lua_tonumber(L, 4);
			const double v_t = lua_tonumber(L, 5);

			lua_pushnumber(L, Lua::Math::CubicBezier(v_c0, v_c1, v_c2, v_c3, v_t));
			return 1;
		}

		void Util::Register(lua_State* L)
		{
			lua_pushstring(L, "util");
			lua_newtable(L);

			Table::PushFunction(L, "lerp", Util::Lerp);
			Table::PushFunction(L, "clamp", Util::Clamp);
			Table::PushFunction(L, "positiveModulo", Util::PositiveModulo);
			Table::PushFunction(L, "smoothstep", Util::Smoothstep);
			Table::PushFunction(L, "smootherstep", Util::Smootherstep);
			Table::PushFunction(L, "bezier2", Util::Bezier2);
			Table::PushFunction(L, "bezier3", Util::Bezier3);

			lua_settable(L, -3);
		}
	}
}