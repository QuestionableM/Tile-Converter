#include "LuaLog.hpp"
#include "CLuaTableUtils.hpp"
#include "BaseLuaFunctions.hpp"

#include "Console.hpp"

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
		int Log::Info(lua_State* L)
		{
			DebugOut("[INFO] ");
			Base::Print(L);

			return 0;
		}

		int Log::Warning(lua_State* L)
		{
			DebugOut(0b1101_fg, "[WARNING] ");
			Base::Print(L);
			DebugOut(0b1110_fg);

			return 1;
		}

		int Log::Error(lua_State* L)
		{
			DebugOut(0b1001_fg, "[ERROR] ");
			Base::Print(L);
			DebugOut(0b1110_fg);

			return 1;
		}

		void Log::Register(lua_State* L)
		{
			lua_pushstring(L, "log");
			lua_newtable(L);

			Table::PushFunction(L, "info", Log::Info);
			Table::PushFunction(L, "warning", Log::Warning);
			Table::PushFunction(L, "error", Log::Error);

			lua_settable(L, -3);
		}
	}
}