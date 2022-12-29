#include "LuaUuid.hpp"
#include "BaseLuaFunctions.hpp"

#include "Utils\Uuid.hpp"

#include "CLuaTableUtils.hpp"
#include "Console.hpp"

extern "C"
{
	#include <lua.h>
	#include <lapi.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace SM
{
	namespace Lua
	{
		inline SMUuid* CreateUuidInternal(lua_State* L)
		{
			SMUuid* v_new_uuid = reinterpret_cast<SMUuid*>(lua_newuserdata(L, sizeof(SMUuid)));
			luaL_setmetatable(L, "Uuid");

			return v_new_uuid;
		}

		int Uuid::New(lua_State* L)
		{
			const int v_num_args = lua_gettop(L);
			if (v_num_args != 1)
			{
				lua_pushfstring(L, "Expected 1 argument, got: %d", v_num_args);
				return lua_error(L);
			}

			const int v_arg_type = lua_type(L, 1);
			if (v_arg_type != LUA_TSTRING)
			{
				lua_pushfstring(L, "Argument 1: string expected, got: %s", lua_typename(L, v_arg_type));
				return lua_error(L);
			}

			const char* v_uuid_str = lua_tostring(L, 1);
			if (strlen(v_uuid_str) != 36)
			{
				lua_pushstring(L, "Invalid UUID string!");
				return lua_error(L);
			}

			SMUuid* v_new_uuid = CreateUuidInternal(L);
			v_new_uuid->FromCString(v_uuid_str);

			return 1;
		}

		int Uuid::GetNil(lua_State* L)
		{
			SMUuid* v_new_uuid = CreateUuidInternal(L);
			v_new_uuid->m_Data64[0] = 0;
			v_new_uuid->m_Data64[1] = 0;

			return 1;
		}

		int Uuid::GenerateNamed(lua_State* L)
		{
			const int v_num_args = lua_gettop(L);
			if (v_num_args != 2)
			{
				lua_pushfstring(L, "Expected 2 arguments, got: %d", v_num_args);
				return lua_error(L);
			}

			SMUuid* v_uuid = reinterpret_cast<SMUuid*>(luaL_testudata(L, 1, "Uuid"));
			if (!v_uuid)
			{
				lua_pushfstring(L, "Uuid expected, got: %s", luaL_typename(L, 1));
				return lua_error(L);
			}

			if (lua_type(L, 2) != LUA_TSTRING)
			{
				lua_pushfstring(L, "String expected, got: %s", luaL_typename(L, 2));
				return lua_error(L);
			}

			const std::string v_str = lua_tostring(L, 2);

			SMUuid* v_new_uuid = CreateUuidInternal(L);
			v_new_uuid->GenerateNamed(*v_uuid, v_str);

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
			SMUuid* v_uuid1 = reinterpret_cast<SMUuid*>(luaL_testudata(L, 1, "Uuid"));
			if (v_uuid1)
			{
				lua_pushboolean(L, v_uuid1->m_Data64[0] == 0 && v_uuid1->m_Data64[1] == 0);
				return 1;
			}

			lua_pushstring(L, "IsNil: called on invalid userdata");
			return lua_error(L);
		}

		void Uuid::Register(lua_State* L)
		{
			{
				//Create uuid table
				lua_pushstring(L, "uuid");
				lua_newtable(L);

				Table::PushFunction(L, "new", Lua::Uuid::New);
				Table::PushFunction(L, "getNil", Lua::Uuid::GetNil);
				Table::PushFunction(L, "generateNamed", Lua::Uuid::GenerateNamed);

				lua_settable(L, -3);
			}

			{
				//Create uuid metatable
				luaL_newmetatable(L, "Uuid");
				Table::PushFunction(L, "__tostring", Uuid::ToString);
				Table::PushFunction(L, "__eq", Uuid::Equals);
				
				{
					lua_pushstring(L, "__index");
					lua_newtable(L);

					Table::PushFunction(L, "isNil", Uuid::IsNil);

					lua_settable(L, -3);
				}

				lua_pop(L, 1);
			}
		}
	}
}