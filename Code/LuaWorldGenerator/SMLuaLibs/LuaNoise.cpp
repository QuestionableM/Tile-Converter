#include "LuaNoise.hpp"

#include "LuaBaseFunctions.hpp"
#include "LuaTableUtils.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

#include <gtc\noise.hpp>
#include <PerlinNoise\PerlinNoise.hpp>
#include <FastNoise\FastNoiseLite.h>

#include <random>

namespace SM
{
	namespace Lua
	{
		template<typename T>
		T lua_int_noise2d(const T& v_noise_x, const T& v_noise_y, const T& v_seed)
		{
			static_assert(std::is_integral_v<T>, "Type must be integral");

			const T v9 = v_noise_x
				+ ((2057
					* ((5 * ((~v_noise_y + (v_noise_y << 15)) ^ ((~v_noise_y + (v_noise_y << 15)) >> 12))) ^ ((5
						* ((~v_noise_y + (v_noise_y << 15)) ^ ((~v_noise_y + (v_noise_y << 15)) >> 12))) >> 4))) ^ ((2057 * ((5 * ((~v_noise_y + (v_noise_y << 15)) ^ ((~v_noise_y + (v_noise_y << 15)) >> 12))) ^ ((5 * ((~v_noise_y + (v_noise_y << 15)) ^ ((~v_noise_y + (v_noise_y << 15)) >> 12))) >> 4))) >> 16));
			const T v10 = 2057
				* ((5 * ((~v9 + (v9 << 15)) ^ ((~v9 + (v9 << 15)) >> 12))) ^ ((5
					* ((~v9 + (v9 << 15)) ^ ((~v9 + (v9 << 15)) >> 12))) >> 4));
			const T v11 = (v10 ^ (v10 >> 16)) + v_seed;
			const T v12 = 2057
				* ((5 * ((~v11 + (v11 << 15)) ^ ((~v11 + (v11 << 15)) >> 12))) ^ ((5
					* ((~v11 + (v11 << 15)) ^ ((~v11 + (v11 << 15)) >> 12))) >> 4));

			return v12 ^ (v12 >> 16);
		}

		int Noise::PerlinNoise2D(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //x
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //y
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //seed

			const double v_x = lua_tonumber(L, 1);
			const double v_y = lua_tonumber(L, 2);

			const int v_seed = static_cast<int>(lua_tonumber(L, 3));
			const double v_noise_output = static_cast<double>(FastNoiseLite{}.SinglePerlin<double>(v_seed, v_x, v_y));

			lua_pushnumber(L, v_noise_output);
			return 1;
		}

		int Noise::IntNoise2D(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //x
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //y
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //seed

			const int v_x = static_cast<int>(lua_tonumber(L, 1));
			const int v_y = static_cast<int>(lua_tonumber(L, 2));
			const int v_seed = static_cast<int>(lua_tonumber(L, 3));
			
			lua_pushinteger(L, lua_int_noise2d(v_x, v_y, v_seed));
			return 1;
		}

		void Noise::Register(lua_State* L)
		{
			lua_pushstring(L, "noise");
			lua_newtable(L);

			Table::PushFunction(L, "perlinNoise2d", Noise::PerlinNoise2D);
			Table::PushFunction(L, "intNoise2d", Noise::IntNoise2D);

			lua_settable(L, -3);
		}
	}
}