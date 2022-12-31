#include "LuaUuid.hpp"

#include "BaseLuaFunctions.hpp"
#include "CLuaTableUtils.hpp"
#include "Console.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lapi.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

namespace SM
{
	namespace Lua
	{
		SMUuid* Uuid::CreateUuid(lua_State* L)
		{
			SMUuid* v_uuid = reinterpret_cast<SMUuid*>(lua_newuserdata(L, sizeof(SMUuid)));
			luaL_setmetatable(L, "Uuid");

			return v_uuid;
		}

		int Uuid::New(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			const char* v_uuid_str = lua_tostring(L, 1);
			if (strlen(v_uuid_str) != 36)
			{
				lua_pushstring(L, "Invalid UUID string!");
				return lua_error(L);
			}

			SMUuid* v_new_uuid = Uuid::CreateUuid(L);
			v_new_uuid->FromCString(v_uuid_str);

			return 1;
		}

		int Uuid::GetNil(lua_State* L)
		{
			SMUuid* v_new_uuid = Uuid::CreateUuid(L);
			v_new_uuid->m_Data64[0] = 0;
			v_new_uuid->m_Data64[1] = 0;

			return 1;
		}

		int Uuid::GenerateNamed(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMUUID);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSTRING);

			SMUuid* v_uuid = reinterpret_cast<SMUuid*>(lua_touserdata(L, 1));
			const std::string v_str = lua_tostring(L, 2);

			SMUuid* v_new_uuid = Uuid::CreateUuid(L);
			v_new_uuid->GenerateNamed(*v_uuid, v_str);

			return 1;
		}

		int Uuid::GenerateRandom(lua_State * L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 0);

			//TODO: Make it actually random later
			SMUuid* v_new_uuid = Uuid::CreateUuid(L);
			return 1;
		}

		int Uuid::ToString(lua_State* L)
		{
			SMUuid* v_uuid_data = reinterpret_cast<SMUuid*>(luaL_testudata(L, 1, "Uuid"));
			if (v_uuid_data)
			{
				const std::string v_uuid_str = v_uuid_data->ToString();
				lua_pushlstring(L, v_uuid_str.data(), v_uuid_str.size());
				return 1;
			}

			return 0;
		}

		int Uuid::Equals(lua_State* L)
		{
			SMUuid* v_uuid1 = reinterpret_cast<SMUuid*>(luaL_testudata(L, 1, "Uuid"));
			SMUuid* v_uuid2 = reinterpret_cast<SMUuid*>(luaL_testudata(L, 2, "Uuid"));

			if (v_uuid1 && v_uuid2)
			{
				lua_pushboolean(L, ((*v_uuid1) == (*v_uuid2)) ? 1 : 0);
				return 1;
			}

			lua_pushboolean(L, 0);
			return 1;
		}

		int Uuid::IsNil(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMUUID);

			SMUuid* v_uuid = reinterpret_cast<SMUuid*>(lua_touserdata(L, 1));
			lua_pushboolean(L, v_uuid->m_Data64[0] == 0 && v_uuid->m_Data64[1] == 0);
			return 1;
		}

		void Uuid::Register(lua_State* L)
		{
			{
				//Create uuid table
				lua_pushstring(L, "uuid");
				lua_newtable(L);

				Table::PushFunction(L, "new", Lua::Uuid::New);
				Table::PushFunction(L, "isNil", Lua::Uuid::IsNil);
				Table::PushFunction(L, "getNil", Lua::Uuid::GetNil);
				Table::PushFunction(L, "generateNamed", Lua::Uuid::GenerateNamed);
				Table::PushFunction(L, "generateRandom", Lua::Uuid::GenerateRandom);

				lua_settable(L, -3);
			}

			{
				//Create uuid metatable
				luaL_newmetatable(L, "Uuid");

				Table::PushPair(L, "__typeid", LUA_TSMUUID);
				Table::PushFunction(L, "__tostring", Lua::Uuid::ToString);
				Table::PushFunction(L, "__eq"      , Lua::Uuid::Equals);
				
				{
					lua_pushstring(L, "__index");
					lua_newtable(L);

					Table::PushFunction(L, "isNil", Lua::Uuid::IsNil);

					lua_settable(L, -3);
				}

				lua_pop(L, 1);
			}
		}
	}
}