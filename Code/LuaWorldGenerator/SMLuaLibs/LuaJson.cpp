#include "LuaJson.hpp"

#include "LuaBaseFunctions.hpp"
#include "LuaTableUtils.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "Console.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Utils\Json.hpp"

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
		nlohmann::json JsonInternal::ConvertElement(lua_State* L)
		{
			switch (Base::Type(L, -1))
			{
			case LUA_TNUMBER:
			{
				if (lua_isinteger(L, -1))
					return nlohmann::json::number_integer_t(lua_tointeger(L, -1));
				else
					return nlohmann::json::number_float_t(lua_tonumber(L, -1));
			}
			case LUA_TSTRING:
				return nlohmann::json::string_t(lua_tostring(L, -1));
			case LUA_TBOOLEAN:
				return nlohmann::json::boolean_t(lua_toboolean(L, -1));
			case LUA_TTABLE:
				{
					nlohmann::json v_object;

					if (JsonInternal::IsTable(L))
					{
						v_object = nlohmann::json::object();
						JsonInternal::IterateTable(L, v_object);
					}
					else
					{
						v_object = nlohmann::json::array();
						JsonInternal::IterateArray(L, v_object);
					}

					return v_object;
				}
			default:
				return nlohmann::json::object();
			}
		}

		/*
		void TraceStack(lua_State* L, const int from, const int to)
		{
			DebugOut("Stack: { ", Base::Typename(Base::Type(L, -1)));
			for (int a = -2; a > -5; a--)
				DebugOut(", ", Base::Typename(Base::Type(L, a)));

			DebugOut(" }\n");
		}
		*/

		void JsonInternal::IterateArray(lua_State* L, nlohmann::json& v_array)
		{
			lua_pushnil(L);

			while (lua_next(L, -2) != 0)
			{
				v_array.push_back(JsonInternal::ConvertElement(L));
				lua_pop(L, 1);
			}
		}

		void JsonInternal::IterateTable(lua_State* L, nlohmann::json& v_object)
		{
			lua_pushnil(L);

			long long v_array_idx = 1;
			while (lua_next(L, -2) != 0)
			{
				const int v_type = lua_type(L, -2);
				switch (lua_type(L, -2))
				{
				case LUA_TSTRING:
					{
						const std::string v_key = lua_tostring(L, -2);
						v_object[v_key] = JsonInternal::ConvertElement(L);
						break;
					}
				case LUA_TNUMBER:
					{
						if (lua_isinteger(L, -2))
							v_object[std::to_string(lua_tointeger(L, -2))] = JsonInternal::ConvertElement(L);
						else
							v_object[std::to_string(lua_tonumber(L, -2))] = JsonInternal::ConvertElement(L);

						break;
					}
				default:
					v_object[std::to_string(v_array_idx)] = JsonInternal::ConvertElement(L);
					break;
				}

				lua_pop(L, 1);
				v_array_idx++;
			}
		}

		bool JsonInternal::IsTable(lua_State* L)
		{
			lua_pushnil(L);

			bool v_is_table = false;
			long long v_order_test = 1;
			while (lua_next(L, -2) != 0)
			{
				if (!lua_isinteger(L, -2))
				{
					v_is_table = true;
				}
				else
				{
					const long long v_cur_idx = lua_tointeger(L, -2);
					if (v_order_test != v_cur_idx)
						v_is_table = true;
				}

				lua_pop(L, 1);
				v_order_test++;
			}

			return v_is_table;
		}

		void JsonInternal::LuaToJson(lua_State* L, nlohmann::json& v_json, const int& v_idx)
		{
			switch (Base::Type(L, v_idx))
			{
			case LUA_TSTRING:
				{
					v_json = nlohmann::json::string_t(lua_tostring(L, v_idx));
					break;
				}
			case LUA_TTABLE:
				{
					if (JsonInternal::IsTable(L))
					{
						v_json = nlohmann::json::object();
						JsonInternal::IterateTable(L, v_json);
					}
					else
					{
						v_json = nlohmann::json::array();
						JsonInternal::IterateArray(L, v_json);
					}

					break;
				}
			case LUA_TNUMBER:
				{
					if (lua_isinteger(L, -1))
						v_json = nlohmann::json::number_integer_t(lua_tointeger(L, -1));
					else
						v_json = nlohmann::json::number_float_t(lua_tonumber(L, -1));

					break;
				}
			case LUA_TBOOLEAN:
				{
					v_json = nlohmann::json::boolean_t(lua_toboolean(L, -1));
					break;
				}
			default:
				v_json = nlohmann::json();
				break;
			}
		}

		void JsonInternal::JsonToLua(lua_State* L, const nlohmann::json& v_json)
		{
			switch (v_json.type())
			{
			case nlohmann::json::value_t::object:
				{
					lua_newtable(L);

					for (const auto& v_obj : v_json.items())
					{
						lua_pushstring(L, v_obj.key().c_str());
						JsonInternal::JsonToLua(L, v_obj.value());

						lua_settable(L, -3);
					}

					break;
				}
			case nlohmann::json::value_t::array:
				{
					lua_newtable(L);

					long long v_arr_idx = 1;
					for (const auto& v_obj : v_json)
					{
						JsonInternal::JsonToLua(L, v_obj);
						lua_rawseti(L, -2, v_arr_idx);

						v_arr_idx++;
					}

					break;
				}
			case nlohmann::json::value_t::number_float:
				{
					lua_pushnumber(L, v_json.get<lua_Number>());
					break;
				}
			case nlohmann::json::value_t::binary:
			case nlohmann::json::value_t::number_integer:
			case nlohmann::json::value_t::number_unsigned:
				{
					lua_pushinteger(L, v_json.get<lua_Integer>());
					break;
				}
			case nlohmann::json::value_t::boolean:
				{
					lua_pushboolean(L, v_json.get<bool>() ? 1 : 0);
					break;
				}
			case nlohmann::json::value_t::string:
				{
					lua_pushstring(L, v_json.get<std::string>().c_str());
					break;
				}
			default:
				lua_pushnil(L);
				break;
			}
		}




		int Json::FileExists(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			std::wstring v_json_path = String::ToWide(lua_tostring(L, 1));
			KeywordReplacer::ReplaceKeyR(v_json_path);

			lua_pushboolean(L, File::Exists(v_json_path) ? 1 : 0);
			return 1;
		}

		int Json::Open(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			const char* v_path_cache = lua_tostring(L, 1);
			std::wstring v_json_path = String::ToWide(v_path_cache);
			KeywordReplacer::ReplaceKeyR(v_json_path);

			nlohmann::json v_f_json;
			std::string v_error_msg;
			if (!JsonReader::LoadParseJsonLua(v_json_path, v_f_json, v_error_msg))
				return luaL_error(L, "Failed to parse %s. Error: %s", v_path_cache, v_error_msg.c_str());

			JsonInternal::JsonToLua(L, v_f_json);
			return 1;
		}

		int Json::Save(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSTRING);

			std::wstring v_json_path = String::ToWide(lua_tostring(L, 2));
			KeywordReplacer::ReplaceKeyR(v_json_path);

			lua_pushvalue(L, 1);

			nlohmann::json v_output_json;
			JsonInternal::LuaToJson(L, v_output_json, -1);
			JsonReader::WriteJson(v_json_path, v_output_json);

			return 0;
		}

		int Json::ParseJsonString(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);

			const std::string v_json_str = lua_tostring(L, 1);

			nlohmann::json v_f_json;
			std::string v_error_msg;
			if (!JsonReader::ParseJsonStringLua(v_json_str, v_f_json, v_error_msg))
				return luaL_error(L, "Failed to parse string. Error: %s", v_error_msg.c_str());

			JsonInternal::JsonToLua(L, v_f_json);
			return 1;
		}

		int Json::WriteJsonString(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);

			lua_pushvalue(L, 1);

			nlohmann::json v_output_json;
			JsonInternal::LuaToJson(L, v_output_json, -1);
			const std::string v_json_str = JsonReader::WriteJsonString(v_output_json);

			lua_pushstring(L, v_json_str.c_str());
			return 1;
		}

		void Json::Register(lua_State* L)
		{
			lua_pushstring(L, "json");
			lua_newtable(L);

			Table::PushFunction(L, "fileExists", Json::FileExists);
			Table::PushFunction(L, "open", Json::Open);
			Table::PushFunction(L, "save", Json::Save);
			Table::PushFunction(L, "parseJsonString", Json::ParseJsonString);
			Table::PushFunction(L, "writeJsonString", Json::WriteJsonString);

			lua_settable(L, -3);
		}
	}
}