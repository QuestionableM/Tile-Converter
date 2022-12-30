#include "LuaColor.hpp"
#include "BaseLuaFunctions.hpp"

#include "Utils\Color.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

using SMColor = ::Color;

namespace SM
{
	namespace Lua
	{
		inline SMColor* CreateColorInternal(lua_State* L)
		{
			SMColor* v_color = reinterpret_cast<SMColor*>(lua_newuserdata(L, sizeof(SMColor)));
			luaL_setmetatable(L, "Color");

			return v_color;
		}

		int Color::New(lua_State* L)
		{
			const int v_arg_count = lua_gettop(L);
			if (v_arg_count != 3 && v_arg_count != 4)
				return luaL_error(L, "Expected 3 or 4 arguments, got: %d", v_arg_count);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			SMColor* v_color = CreateColorInternal(L);

			return 1;
		}

		void Color::Register(lua_State* L)
		{
			{
				//Register the color table
				lua_pushstring(L, "color");
				lua_newtable(L);

				lua_settable(L, -3);
			}

			{
				//Register the color metatable
				luaL_newmetatable(L, "Color");

				//Init it later

				lua_pop(L, 1);
			}
		}
	}
}