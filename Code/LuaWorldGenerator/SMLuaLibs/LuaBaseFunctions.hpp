#pragma once

#include <string>

extern "C"
{
	#include <lua\lstate.h>
}

#define LUA_TSMVEC3 (LUA_TOTALTYPES + 1)
#define LUA_TSMQUAT (LUA_TOTALTYPES + 2)
#define LUA_TSMUUID (LUA_TOTALTYPES + 3)
#define LUA_TSMCOLOR (LUA_TOTALTYPES + 4)
#define LUA_SM_NUM_TYPES (LUA_TOTALTYPES + 5)

#define G_LUA_CUSTOM_ARG_CHECK(L, N) \
	const int v_num_args = lua_gettop(L); \
	if (v_num_args != N) { \
		return luaL_error(L, "Expected %d arguments, got: %d", N, v_num_args); \
	}

#define G_LUA_CUSTOM_ARG_TYPE_ERROR(L, I, T) \
	return luaL_error(L, "Argument %d: %s expected, got: %s", I, SM::Lua::Base::Typename(T), SM::Lua::Base::Typename(SM::Lua::Base::Type(L, I)))

#define G_LUA_CUSTOM_ARG_TYPE_CHECK(L, I, T) \
	if (SM::Lua::Base::Type(L, I) != T) { \
		G_LUA_CUSTOM_ARG_TYPE_ERROR(L, I, T); \
	}

#define G_LUA_REPLACE_KEY_CHECKED(L, v_path) \
	if (!KeywordReplacer::ReplaceKeyRLua(v_path)) \
		return luaL_error(L, "Path contains $CONTENT_DATA, but it is not defined!")

namespace SM
{
	namespace Lua
	{
	#if defined(_DEBUG) || defined(DEBUG)
		namespace PrintInternal
		{
			void PrintTableElement(lua_State* L);
			void PrintTable(lua_State* L);
			void PrintArray(lua_State* L);

			//push the element onto the stack before printing it
			void PrintCustomType(lua_State* L);
			void PrintBase(lua_State* L);
		}
	#endif

		struct Base
		{
			static int Error(lua_State* L);
			static int Assert(lua_State* L);
			//Loads the contents of lua file into the current environment
			static int Dofile(lua_State* L);
			static int TypeLua(lua_State* L);
			static int Print(lua_State* L);
			static int ToString(lua_State* L);
			static int ToNumber(lua_State* L);
			static int IPairs(lua_State* L);
			static int Next(lua_State* L);
			static int Pairs(lua_State* L);
			static int PCall(lua_State* L);
			static int Select(lua_State* L);
			static int XPCall(lua_State* L);

			//Checks if the value at stack id -1 is a table or an array
			static bool IsTable(lua_State* L);

			//Returns the type id of an object
			static int Type(lua_State* L, const int& idx);
			//Gets the type name string of an object
			static const char* Typename(const int& idx);

			//Registers all the lua base functions
			static void Register(lua_State* L);

			inline static const std::wstring& GetCurrentFile() { return Base::m_CurrentLuaFile; }
			inline static void SetCurrentFile(const std::wstring& file) { Base::m_CurrentLuaFile = file; }

		private:
			inline static std::wstring m_CurrentLuaFile = L"";
		};
	}
}