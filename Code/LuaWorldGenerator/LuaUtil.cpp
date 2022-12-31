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

namespace SM
{
	namespace Lua
	{
		template<typename T>
		inline T lua_lerp(const T& a, const T& b, const T& f)
		{
			static_assert(std::is_floating_point_v<T>, "lua_lerp can only be used with floating point values");

			return a * (static_cast<T>(1.0) - f) + (b * f);
		}

		int Util::Lerp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //first val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //second val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //interpolation step

			const double v_first_val = lua_tonumber(L, 1);
			const double v_second_val = lua_tonumber(L, 2);
			const double v_step = lua_tonumber(L, 3);

			lua_pushnumber(L, lua_lerp(v_first_val, v_second_val, v_step));
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

		void Util::Register(lua_State* L)
		{
			lua_pushstring(L, "util");
			lua_newtable(L);

			Table::PushFunction(L, "lerp", Util::Lerp);
			Table::PushFunction(L, "clamp", Util::Clamp);
			Table::PushFunction(L, "positiveModulo", Util::PositiveModulo);

			lua_settable(L, -3);
		}
	}
}