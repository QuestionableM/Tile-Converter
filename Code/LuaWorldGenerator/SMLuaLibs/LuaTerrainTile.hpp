#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace TerrainTile
		{
			int GetTileUuid(lua_State* L);

			int GetSize(lua_State* L);

			int GetCreatorId(lua_State* L);

			void Register(lua_State* L);
		}
	}
}