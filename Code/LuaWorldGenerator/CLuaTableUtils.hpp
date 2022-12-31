#pragma once

#include <string>

extern "C"
{
	#include <lua\lstate.h>
}


namespace SM
{
	namespace Lua
	{
		template<class T>
		inline constexpr void PushValue(lua_State* L, const T value)
		{
			static_assert(
				std::is_arithmetic_v<T> ||
				std::is_same_v<T, const char*> ||
				std::is_same_v<T, std::string>,
				"Only the specified types can be used: any arithmetic type, const char*, std::string");

			if constexpr (std::is_same_v<T, bool>)
				lua_pushboolean(L, value ? 1 : 0);
			else if constexpr (std::is_integral_v<T>)
				lua_pushinteger(L, static_cast<long long>(value));
			else if constexpr (std::is_floating_point_v<T>)
				lua_pushnumber(L, static_cast<double>(value));
			else if constexpr (std::is_same_v<T, const char*>)
				lua_pushstring(L, value);
			else if constexpr (std::is_same_v<T, std::string>)
				lua_pushstring(L, value.c_str());
		}

		namespace Table
		{
			template<class K, class V>
			inline constexpr void PushPair(lua_State* L, const K key, const V value)
			{
				Lua::PushValue<K>(L, key);
				Lua::PushValue<V>(L, value);

				lua_settable(L, -3);
			}

			inline void PushFunction(lua_State* L, const char* key, lua_CFunction func)
			{
				lua_pushstring(L, key);
				lua_pushcfunction(L, func);

				lua_settable(L, -3);
			}
		}
	}
}