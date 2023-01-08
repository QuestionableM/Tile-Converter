#pragma once

#include "Utils\Json.hpp"

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

			//Requires the value to be pushed onto the stack
			void LuaToJson(lua_State* L, nlohmann::json& v_json, const int& v_idx);
			void JsonToLua(lua_State* L, const simdjson::dom::element& v_element);
		}

		namespace Json
		{
			int FileExists(lua_State* L);

			int Open(lua_State* L);
			int Save(lua_State* L);
			int ParseJsonString(lua_State* L);
			int WriteJsonString(lua_State* L);

			void Register(lua_State* L);
		}
	}
}