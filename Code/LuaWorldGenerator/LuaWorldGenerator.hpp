#pragma once

#include <string>

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	#include <lstate.h>
}

namespace SM
{
	class LuaWorldGenerator
	{
	public:
		LuaWorldGenerator();
		LuaWorldGenerator(const LuaWorldGenerator&&) = delete;
		LuaWorldGenerator(LuaWorldGenerator&&) = delete;
		~LuaWorldGenerator();

		void RegisterFunctions();

		void Load(const std::wstring& path, const std::wstring& name);

	private:
		lua_State* m_lState;
	};
}