#pragma once

#include <nlohmann\json.hpp>

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace JsonInternal
		{
			nlohmann::json ConvertElement(lua_State* L);

			void IterateArray(lua_State* L, nlohmann::json& v_array);
			void IterateTable(lua_State* L, nlohmann::json& v_object);

			bool IsTable(lua_State* L);

			void LuaToJson(lua_State* L, nlohmann::json& v_json, const int& v_idx);
			void JsonToLua(lua_State* L, const nlohmann::json& v_json);
		}

		namespace Json
		{
			int FileExists(lua_State* L);

			int Open(lua_State* L);
			int Save(lua_State* L);

			void Register(lua_State* L);
		}
	}
}