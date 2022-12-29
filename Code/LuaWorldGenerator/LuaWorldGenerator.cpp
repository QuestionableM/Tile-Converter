#include "LuaWorldGenerator.hpp"
#include "BaseLuaFunctions.hpp"
#include "LuaUuid.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Console.hpp"

namespace SM
{
	LuaWorldGenerator::LuaWorldGenerator()
	{
		m_lState = luaL_newstate();
	}

	LuaWorldGenerator::~LuaWorldGenerator()
	{
		if (m_lState) lua_close(m_lState);
	}

	/*
LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
	for (lib = loadedlibs; lib->func; lib++) {
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1); 
	}
}
	*/

	/*
	{"assert", luaB_assert},
		{"collectgarbage", luaB_collectgarbage},
		{"dofile", luaB_dofile},
		{"error", luaB_error},
		{"getmetatable", luaB_getmetatable},
		{"ipairs", luaB_ipairs},
		{"loadfile", luaB_loadfile},
		{"load", luaB_load},
		{"next", luaB_next},
		{"pairs", luaB_pairs},
		{"pcall", luaB_pcall},
		{"print", luaB_print},
		{"warn", luaB_warn},
		{"rawequal", luaB_rawequal},
		{"rawlen", luaB_rawlen},
		{"rawget", luaB_rawget},
		{"rawset", luaB_rawset},
		{"select", luaB_select},
		{"setmetatable", luaB_setmetatable},
		{"tonumber", luaB_tonumber},
		{"tostring", luaB_tostring},
		{"type", luaB_type},
		{"xpcall", luaB_xpcall},
	*/

	void LuaWorldGenerator::RegisterSMTable()
	{
		lua_newtable(m_lState);

		Lua::Uuid::Register(m_lState);

		//Push SM namespace into the global environment
		lua_setglobal(m_lState, "sm");
	}

#define G_LUA_ERROR_CHECK(val, lua_state) \
	if ((val) != LUA_OK) { \
		DebugErrorL("Lua Error: ", lua_tostring(lua_state, -1)); \
		return; \
	} 

	void LuaWorldGenerator::Load(const std::wstring& path, const std::wstring& name)
	{
		if (m_lState == nullptr)
		{
			DebugOutL("Couldn't create a lua state!");
			return;
		}

		std::string v_file_data;
		if (!File::ReadToStringNormal(path, v_file_data))
		{
			DebugOutL("Couldn't read the specified lua file");
			return;
		}

		Lua::Base::Register(m_lState);
		this->RegisterSMTable();

		Lua::Base::SetCurrentFile(path);

		//Load the main lua file
		G_LUA_ERROR_CHECK(luaL_dostring(m_lState, v_file_data.c_str()), m_lState);

		lua_getglobal(m_lState, "Init");
		if (!lua_isfunction(m_lState, -1))
		{
			DebugOutL("Couldn't find the terrain generator init function!");
			return;
		}

		//Call the init function
		G_LUA_ERROR_CHECK(lua_pcall(m_lState, 0, 0, 0), m_lState);

		//Find the create function and call it
		lua_getglobal(m_lState, "Create");
		if (!lua_isfunction(m_lState, -1))
		{
			DebugOutL("Couldn't find the terrain generator entry point!");
			return;
		}
		
		//Push arguments
		{
			lua_pushinteger(m_lState, 1);
			lua_pushinteger(m_lState, 2);
			lua_pushinteger(m_lState, 3);
			lua_pushinteger(m_lState, 4);
			lua_pushinteger(m_lState, static_cast<long long>(rand()));

			G_LUA_ERROR_CHECK(lua_pcall(m_lState, 5, 0, 0), m_lState);
		}
	}
}