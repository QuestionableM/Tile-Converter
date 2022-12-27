#include "LuaWorldGenerator.hpp"

#include "Utils\File.hpp"
#include "Console.hpp"

namespace SM
{
	LuaWorldGenerator::LuaWorldGenerator()
	{
		m_lState = luaL_newstate();
	}

	LuaWorldGenerator::~LuaWorldGenerator()
	{
		if (m_lState) lua_close(m_lState);
	}

	/*
	static const luaL_Reg loadedlibs[] = {
  {LUA_GNAME, luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
	for (lib = loadedlibs; lib->func; lib++) {
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1); 
	}
}
	*/

	/*
	static const luaL_Reg base_funcs[] = {
		{"assert", luaB_assert},
		{"collectgarbage", luaB_collectgarbage},
		{"dofile", luaB_dofile},
		{"error", luaB_error},
		{"getmetatable", luaB_getmetatable},
		{"ipairs", luaB_ipairs},
		{"loadfile", luaB_loadfile},
		{"load", luaB_load},
		{"next", luaB_next},
		{"pairs", luaB_pairs},
		{"pcall", luaB_pcall},
		{"print", luaB_print},
		{"warn", luaB_warn},
		{"rawequal", luaB_rawequal},
		{"rawlen", luaB_rawlen},
		{"rawget", luaB_rawget},
		{"rawset", luaB_rawset},
		{"select", luaB_select},
		{"setmetatable", luaB_setmetatable},
		{"tonumber", luaB_tonumber},
		{"tostring", luaB_tostring},
		{"type", luaB_type},
		{"xpcall", luaB_xpcall},

		{LUA_GNAME, NULL},
		{ "_VERSION", NULL },
		{ NULL, NULL }
	};


	LUAMOD_API int luaopen_base(lua_State* L) {
		// open lib into global table
		lua_pushglobaltable(L);
		luaL_setfuncs(L, base_funcs, 0);
		// set global _G
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, LUA_GNAME);
		// set global _VERSION
		lua_pushliteral(L, LUA_VERSION);
		lua_setfield(L, -2, "_VERSION");
		return 1;
	}
*/
	void print_table(lua_State* L)
	{
		lua_pushnil(L);

		
		//while (lua_next(L, -2) != 0)
		//{
			//if (lua_isstring(L, -1))
		//}
		/*
		void PrintTable(lua_State *L)
		{
			lua_pushnil(L);

			while(lua_next(L, -2) != 0)
			{
				if(lua_isstring(L, -1))
				  printf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
				else if(lua_isnumber(L, -1))
				  printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
				else if(lua_istable(L, -1))
				  PrintTable(L);

				lua_pop(L, 1);
			}
		}
		*/
	}

	/*const char* custom_tostring(lua_State* L, int idx, size_t* len)
	{
		idx = lua_absindex(L, idx);
		if (luaL_callmeta(L, idx, "__tostring"))
		{
			if (!lua_isstring(L, -1))
				luaL_error(L, "'__tostring' must return a string");
		}
		else
		{
			switch (lua_type(L, idx))
			{
			case LUA_TNUMBER:
				{
					if (lua_isinteger(L, idx))
						lua_pushfstring(L, "%I", (LUAI_UACINT)lua_tointeger(L, idx));
					else
						lua_pushfstring(L, "%f", (LUAI_UACNUMBER)lua_tonumber(L, idx));

					break;
				}
			case LUA_TSTRING:
				lua_pushvalue(L, idx);
				break;
			case LUA_TBOOLEAN:
				lua_pushstring(L, (lua_toboolean(L, idx) ? "true" : "false"));
				break;
			case LUA_TNIL:
				lua_pushliteral(L, "nil");
				break;
			default:
				{
					int tt = luaL_getmetafield(L, idx, "__name");  /* try name */
					//const char* kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) :
					//	luaL_typename(L, idx);
					//lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, idx));
					//if (tt != LUA_TNIL)
					//	lua_remove(L, -2);  /* remove '__name' */
					//break;
				//}
			//}
		//}

		//return lua_tolstring(L, -1, len);
	//}
#if defined(_DEBUG)
extern "C"
{
	/*
		// In Lua 5.0 reference manual is a table traversal example at page 29.
		void PrintTable(lua_State *L)
		{
			lua_pushnil(L);

			while(lua_next(L, -2) != 0)
			{
				if(lua_isstring(L, -1))
				  printf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
				else if(lua_isnumber(L, -1))
				  printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
				else if(lua_istable(L, -1))
				  PrintTable(L);

				lua_pop(L, 1);
			}
		}
	*/
	void custom_print_table(lua_State* L, int idx);

	void print_table_element(lua_State* L)
	{
		switch (lua_type(L, -1))
		{
		case LUA_TNUMBER:
			{
				if (lua_isinteger(L, -1))
					DebugOut((LUAI_UACINT)lua_tointeger(L, -1));
				else
					DebugOut((LUAI_UACNUMBER)lua_tonumber(L, -1));

				break;
			}
		case LUA_TBOOLEAN:
			DebugOut(lua_toboolean(L, -1) ? "true" : "false");
			break;
		case LUA_TSTRING:
			DebugOut(lua_tostring(L, -1));
			break;
		case LUA_TNIL:
			DebugOut("nil");
			break;
		case LUA_TTABLE:
			custom_print_table(L, -2);
			break;
		default:
			{
				const int tt = luaL_getmetafield(L, -1, "__name"); //try name
				const char* kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) : luaL_typename(L, -1);
				lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, -1));
				if (tt != LUA_TNIL)
					lua_remove(L, -2); //remove __name

				DebugOut(lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			}
		}
	}

	inline void print_table_internal(lua_State* L)
	{
		if (lua_isnumber(L, -2))
		{
			print_table_element(L);
		}
		else
		{
			DebugOut(lua_tostring(L, -2), " = ");
			print_table_element(L);
		}

		lua_pop(L, 1);
	}

	void custom_print_table(lua_State* L, int idx)
	{
		lua_pushnil(L);

		DebugOut("{ ");

		if (lua_next(L, idx) != 0)
			print_table_internal(L);

		while (lua_next(L, idx) != 0)
		{
			DebugOut(", ");
			print_table_internal(L);
		}

		DebugOut(" }");
	}

	void custom_print_string(lua_State* L, int idx)
	{
		switch (lua_type(L, idx))
		{
		case LUA_TNUMBER:
			{
				if (lua_isinteger(L, idx))
					DebugOut((LUAI_UACINT)lua_tointeger(L, idx));
				else
					DebugOut((LUAI_UACNUMBER)lua_tonumber(L, idx));

				break;
			}
		case LUA_TSTRING:
			DebugOut(lua_tostring(L, idx));
			break;
		case LUA_TBOOLEAN:
			DebugOut(lua_toboolean(L, idx) ? "true" : "false");
			break;
		case LUA_TNIL:
			DebugOut("nil");
			break;
		case LUA_TTABLE:
			custom_print_table(L, idx);
			break;
		default:
			{
				const int tt = luaL_getmetafield(L, idx, "__name"); //try name
				const char* kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) : luaL_typename(L, idx);
				lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, idx));
				if (tt != LUA_TNIL)
					lua_remove(L, -2); //remove __name

				DebugOut(lua_tostring(L, -1));
				break;
			}
		}
	}
}
#endif

	extern "C" int custom_print(lua_State* L)
	{
	#if defined(_DEBUG)
		const int n = lua_gettop(L); //number of arguments

		custom_print_string(L, 1);

		for (int i = 2; i <= n; i++)
		{
			DebugOut(" ");
			custom_print_string(L, i);
		}

		DebugOut("\n");
	#endif

		return 0;
	}

	static const luaL_Reg g_base_functions[] =
	{
		{ "print", custom_print },
		{ LUA_GNAME, NULL },
		{ "_VERSION", NULL },
		{ NULL, NULL }
	};

	void LuaWorldGenerator::RegisterFunctions()
	{
		lua_pushglobaltable(m_lState);
		luaL_setfuncs(m_lState, g_base_functions, 0);

		lua_pushvalue(m_lState, -1);
		lua_setfield(m_lState, -2, LUA_GNAME);

		lua_pushliteral(m_lState, LUA_VERSION);
		lua_setfield(m_lState, -2, "_VERSION");
	}

	void LuaWorldGenerator::Load(const std::wstring& path, const std::wstring& name)
	{
		if (m_lState == nullptr)
		{
			DebugOutL("Couldn't create a lua state!");
			return;
		}

		std::string v_file_data;
		if (!File::ReadToStringNormal(path, v_file_data))
		{
			DebugOutL("Couldn't read the specified lua file");
			return;
		}

		this->RegisterFunctions();

		int v_result = luaL_dostring(m_lState, v_file_data.c_str());
		if (v_result != LUA_OK)
		{
			std::string v_error_msg = lua_tostring(m_lState, -1);
			DebugOutL("Failed to load lua string\nError: ", v_error_msg);
			return;
		}
	}
}