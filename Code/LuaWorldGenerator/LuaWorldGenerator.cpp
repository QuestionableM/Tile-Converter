#include "LuaWorldGenerator.hpp"

#include "SMLuaLibs\LuaBaseFunctions.hpp"
#include "SMLuaLibs\LuaTerrainTile.hpp"
#include "SMLuaLibs\LuaTerrainData.hpp"
#include "SMLuaLibs\LuaTableUtils.hpp"
#include "SMLuaLibs\LuaColor.hpp"
#include "SMLuaLibs\LuaNoise.hpp"
#include "SMLuaLibs\LuaUuid.hpp"
#include "SMLuaLibs\LuaVec3.hpp"
#include "SMLuaLibs\LuaUtil.hpp"
#include "SMLuaLibs\LuaQuat.hpp"
#include "SMLuaLibs\LuaJson.hpp"
#include "SMLuaLibs\LuaLog.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "Console.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"

#include <random>

#define G_LUA_ERROR_CHECK(val, lua_state)                      \
	if ((val) != LUA_OK) {                                     \
		const char* v_error_str = lua_tostring(lua_state, -1); \
		if (!v_error_str) {                                    \
			DebugErrorL("Lua Error: Unknown Error");           \
			return;                                            \
		}                                                      \
		DebugErrorL("Lua Error: ", v_error_str);               \
		return;                                                \
	}

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

	static int Lua_AlwaysTrueFunction(lua_State* L)
	{
		lua_pushboolean(L, 1);
		return 1;
	}

	void LuaWorldGenerator::RegisterSMTable()
	{
		lua_newtable(m_lState);

		{
			//Register functions that belong to the root of sm table
			Lua::Table::PushFunction(m_lState, "exists", Lua_AlwaysTrueFunction);
			Lua::Table::PushFunction(m_lState, "isServerMode", Lua_AlwaysTrueFunction);
			Lua::Table::PushPair(m_lState, "version", "0.0.0.000");
			Lua::Table::PushPair(m_lState, "isHost", true);
		}

		Lua::Uuid::Register(m_lState);
		Lua::Vec3::Register(m_lState);
		Lua::Color::Register(m_lState);
		Lua::TerrainTile::Register(m_lState);
		Lua::TerrainData::Register(m_lState);
		Lua::Util::Register(m_lState);
		Lua::Quat::Register(m_lState);
		Lua::Log::Register(m_lState);
		Lua::Json::Register(m_lState);
		Lua::Noise::Register(m_lState);

		//Push SM namespace into the global environment
		lua_setglobal(m_lState, "sm");
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
			//xMin, xMax, yMin, yMax, seed, data
			lua_pushinteger(m_lState, -64); //xMin
			lua_pushinteger(m_lState, 63); //xMax
			lua_pushinteger(m_lState, -48); //yMin
			lua_pushinteger(m_lState, 47); //yMax

			std::random_device v_randomDevice;
			std::mt19937_64 v_randomGenerator(v_randomDevice());
			std::uniform_int_distribution<int> v_randomDistribution;

			lua_pushinteger(m_lState, static_cast<long long>(v_randomDistribution(v_randomGenerator))); //seed

			G_LUA_ERROR_CHECK(lua_pcall(m_lState, 5, 0, 0), m_lState);
		}
	}
}