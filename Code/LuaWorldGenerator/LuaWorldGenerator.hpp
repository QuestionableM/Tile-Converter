#pragma once

#include <string>

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
	#include <lua\lstate.h>
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

		void RegisterSMTable();

		void Load(const std::wstring& path, const std::wstring& name);

	private:
		lua_State* m_lState;
	};
}