#include "LuaVec3.hpp"

#include "BaseLuaFunctions.hpp"
#include "CLuaTableUtils.hpp"
#include "Console.hpp"

#include <unordered_map>

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lauxlib.h>
}

#include <glm.hpp>

namespace SM
{
	namespace Lua
	{
		inline glm::vec3* CreateVec3Internal(lua_State* L)
		{
			glm::vec3* v_new_vec = reinterpret_cast<glm::vec3*>(lua_newuserdata(L, sizeof(glm::vec3)));
			luaL_setmetatable(L, "Vec3");

			return v_new_vec;
		}

		int Vec3::New(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			glm::vec3* v_new_uuid = CreateVec3Internal(L);
			v_new_uuid->x = static_cast<float>(lua_tonumber(L, 1));
			v_new_uuid->y = static_cast<float>(lua_tonumber(L, 2));
			v_new_uuid->z = static_cast<float>(lua_tonumber(L, 3));

			return 1;
		}

		int Vec3::Zero(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 0);

			glm::vec3* v_new_uuid = CreateVec3Internal(L);
			v_new_uuid->x = 0;
			v_new_uuid->y = 0;
			v_new_uuid->z = 0;

			return 1;
		}

		int Vec3::One(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 0);

			glm::vec3* v_new_uuid = CreateVec3Internal(L);
			v_new_uuid->x = 1;
			v_new_uuid->y = 1;
			v_new_uuid->z = 1;

			return 1;
		}

		int Vec3::Equals(lua_State* L)
		{
			glm::vec3* v_vec1 = reinterpret_cast<glm::vec3*>(luaL_testudata(L, 1, "Vec3"));
			glm::vec3* v_vec2 = reinterpret_cast<glm::vec3*>(luaL_testudata(L, 2, "Vec3"));

			if (v_vec1 && v_vec2)
			{
				lua_pushboolean(L, (*v_vec1) == (*v_vec2));
				return 1;
			}

			lua_pushboolean(L, 0);
			return 1;
		}

		int Vec3::Normalize(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
		
			glm::vec3* v_new_vec = CreateVec3Internal(L);
			(*v_new_vec) = glm::normalize(*reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1)));

			return 1;
		}

		int Vec3::Cross(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMVEC3);

			glm::vec3* v_vec1 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			glm::vec3* v_vec2 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 2));

			glm::vec3* v_new_vec = CreateVec3Internal(L);
			(*v_new_vec) = glm::cross(*v_vec1, *v_vec2);

			return 1;
		}

		int Vec3::GetX(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->x));
			return 1;
		}

		int Vec3::GetY(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->y));
			return 1;
		}

		int Vec3::GetZ(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->z));
			return 1;
		}

		int Vec3::SetX(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			v_vec->x = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->x));
			return 1;
		}

		int Vec3::SetY(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			v_vec->y = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->y));
			return 1;
		}

		int Vec3::SetZ(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			v_vec->z = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_vec->z));
			return 1;
		}

		int Vec3::ToString(lua_State* L)
		{
			glm::vec3* v_vec = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			lua_pushfstring(L, "(%f,%f,%f)", v_vec->x, v_vec->y, v_vec->z);

			return 1;
		}

		int Vec3::NewIndex(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			glm::vec3* v_userdata = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			const char* v_index_str = lua_tostring(L, 2);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			switch (*v_index_str)
			{
			case 'x': v_userdata->x = static_cast<float>(lua_tonumber(L, 3)); return 0;
			case 'y': v_userdata->y = static_cast<float>(lua_tonumber(L, 3)); return 0;
			case 'z': v_userdata->z = static_cast<float>(lua_tonumber(L, 3)); return 0;
			}

			return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);
		}

		static const std::unordered_map<std::string, int (*)(lua_State*)> g_vec3Userdata =
		{
			{ "normalize", Vec3::Normalize },
			{ "cross"    , Vec3::Cross     }
		};

		int Vec3::Index(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);

			glm::vec3* v_vec3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
			const char* v_index_str = lua_tostring(L, 2);

			switch (*v_index_str)
			{
			case 'x': lua_pushinteger(L, static_cast<lua_Integer>(v_vec3->x)); return 1;
			case 'y': lua_pushinteger(L, static_cast<lua_Integer>(v_vec3->y)); return 1;
			case 'z': lua_pushinteger(L, static_cast<lua_Integer>(v_vec3->z)); return 1;
			}

			const auto v_iter = g_vec3Userdata.find(v_index_str);
			if (v_iter == g_vec3Userdata.end())
				return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);

			lua_pushcfunction(L, v_iter->second);
			return 1;
		}

		void Vec3::Register(lua_State* L)
		{
			{
				//Create vec3 table
				lua_pushstring(L, "vec3");
				lua_newtable(L);

				Table::PushFunction(L, "new", Lua::Vec3::New);
				Table::PushFunction(L, "zero", Lua::Vec3::Zero);
				Table::PushFunction(L, "one", Lua::Vec3::One);
				Table::PushFunction(L, "normalize", Lua::Vec3::Normalize);
				Table::PushFunction(L, "cross", Lua::Vec3::Cross);

				Table::PushFunction(L, "getX", Lua::Vec3::GetX);
				Table::PushFunction(L, "getY", Lua::Vec3::GetY);
				Table::PushFunction(L, "getZ", Lua::Vec3::GetZ);

				Table::PushFunction(L, "setX", Lua::Vec3::SetX);
				Table::PushFunction(L, "setY", Lua::Vec3::SetY);
				Table::PushFunction(L, "setZ", Lua::Vec3::SetZ);

				lua_settable(L, -3);
			}

			{
				//Create the vec3 metatable
				luaL_newmetatable(L, "Vec3");

				Table::PushPair(L, "__typeid", LUA_TSMVEC3);

				Table::PushFunction(L, "__eq", Lua::Vec3::Equals);
				Table::PushFunction(L, "__newindex", Lua::Vec3::NewIndex);
				Table::PushFunction(L, "__index", Lua::Vec3::Index);

				lua_pop(L, 1); //pop the metatable off the stack
			}
		}
	}
}