#include "LuaTerrainTile.hpp"

#include "LuaBaseFunctions.hpp"
#include "LuaTableUtils.hpp"
#include "LuaUuid.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
//TODO: Make it so the order in those 2 files wouldn't matter
#include "Tile\TileConverter.hpp"
#include "SMReaders\TileReader.hpp"
#include "Utils\String.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lauxlib.h>
	#include <lua\lualib.h>
}

namespace SM
{
	namespace Lua
	{
		int ReadTileHeaderWrapped(lua_State* L, const std::wstring& path, TileHeaderBaseInfo& v_header)
		{
			ConvertError v_error;
			TileHeaderBaseInfo v_base_info;
			if (!TileReader::ReadTileHeader(path, v_base_info, v_error))
			{
				const std::string v_tile_path_str = String::ToUtf8(path);
				const std::string v_error_str = String::ToUtf8(v_error.GetErrorMsg());

				return luaL_error(L, "Couldn't read tile: %s, Error: %s", v_tile_path_str, v_error_str);
			}

			return 1;
		}

		int TerrainTile::GetTileUuid(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			std::wstring v_tile_path = String::ToWide(lua_tostring(L, 1));
			G_LUA_REPLACE_KEY_CHECKED(L, v_tile_path);

			TileHeaderBaseInfo v_base_info;
			if (ReadTileHeaderWrapped(L, v_tile_path, v_base_info) == 0)
				return 0;

			SMUuid* v_uuid = Uuid::CreateUuid(L);
			(*v_uuid) = v_base_info.uuid;

			return 1;
		}

		int TerrainTile::GetSize(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			std::wstring v_tile_path = String::ToWide(lua_tostring(L, 1));
			G_LUA_REPLACE_KEY_CHECKED(L, v_tile_path);

			TileHeaderBaseInfo v_base_info;
			if (ReadTileHeaderWrapped(L, v_tile_path, v_base_info) == 0)
				return 0;

			lua_pushinteger(L, static_cast<lua_Integer>(v_base_info.width));
			return 1;
		}

		int TerrainTile::GetCreatorId(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			std::wstring v_tile_path = String::ToWide(lua_tostring(L, 1));
			G_LUA_REPLACE_KEY_CHECKED(L, v_tile_path);

			TileHeaderBaseInfo v_base_info;
			if (ReadTileHeaderWrapped(L, v_tile_path, v_base_info) == 0)
				return 0;

			lua_pushinteger(L, static_cast<lua_Integer>(v_base_info.creator_id));
			return 1;
		}

		void TerrainTile::Register(lua_State* L)
		{
			lua_pushstring(L, "terrainTile");
			lua_newtable(L);

			Table::PushFunction(L, "getTileUuid", TerrainTile::GetTileUuid);
			Table::PushFunction(L, "getSize", TerrainTile::GetSize);
			Table::PushFunction(L, "getCreatorId", TerrainTile::GetCreatorId);

			lua_settable(L, -3);
		}
	}
}