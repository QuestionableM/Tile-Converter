#include "LuaTerrainData.hpp"

#include "LuaTableUtils.hpp"

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
		int lua_empty_function(lua_State* L)
		{
			return 0;
		}

		void TerrainData::Register(lua_State* L)
		{
			lua_pushstring(L, "terrainData");
			lua_newtable(L);

			Table::PushFunction(L, "save", lua_empty_function);
			Table::PushFunction(L, "load", lua_empty_function);
			Table::PushFunction(L, "exists", lua_empty_function);
			Table::PushFunction(L, "legacy_setData", lua_empty_function);
			Table::PushFunction(L, "legacy_getData", lua_empty_function);
			Table::PushFunction(L, "legacy_saveTerrainData", lua_empty_function);
			Table::PushFunction(L, "legacy_loadTerrainData", lua_empty_function);

			lua_settable(L, -3);
		}
	}
}