#include "BaseLuaFunctions.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "Utils\String.hpp"
#include "Console.hpp"

#include <string>

extern "C"
{
	#include <lapi.h>
	#include <lua.h>
	#include <lauxlib.h>
	#include <luaconf.h>
}

namespace SM
{
	namespace Lua
	{
		int Base::Error(lua_State* L)
		{
			const int level = (int)luaL_optinteger(L, 2, 1);
			lua_settop(L, 1);

			if (lua_type(L, 1) == LUA_TSTRING && level > 0)
			{
				luaL_where(L, level);   // add extra information
				lua_pushvalue(L, 1);
				lua_concat(L, 2);
			}

			return lua_error(L);
		}

		int Base::Assert(lua_State* L)
		{
			if (luai_likely(lua_toboolean(L, 1)))  /* condition is true? */
				return lua_gettop(L);  /* return all arguments */
			else {  /* error */
				luaL_checkany(L, 1);  /* there must be a condition */
				lua_remove(L, 1);  /* remove it */
				lua_pushliteral(L, "assertion failed!");  /* default message */
				lua_settop(L, 1);  /* leave only message (default if no other one) */
				return Lua::Base::Error(L);  /* call 'error' */
			}
		}

		static int dofilecont(lua_State* L, int d1, lua_KContext d2) {
			(void)d1;  (void)d2;  /* only to match 'lua_Kfunction' prototype */
			return lua_gettop(L) - 1;
		}

		int Base::Dofile(lua_State* L)
		{
			std::wstring v_file_path = String::ToWide(luaL_optstring(L, 1, NULL));
			KeywordReplacer::ReplaceKeyR(v_file_path);

			DebugOutL("Dofile: ", v_file_path);

			lua_settop(L, 1);

			if (luaL_loadfile(L, String::ToUtf8(v_file_path).c_str()) != LUA_OK)
				return lua_error(L);

			lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
			return dofilecont(L, 0, 0);
		}

		int Base::Type(lua_State* L)
		{
			const int t = lua_type(L, 1);
			luaL_argcheck(L, t != LUA_TNONE, 1, "value expected");

			const int tt = luaL_getmetafield(L, 1, "__name"); //try name
			const char* kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) : lua_typename(L, t);

			if (tt != LUA_TNIL)
				lua_remove(L, -2); //remove __name

			lua_pushstring(L, kind);
			return 1;
		}

	#if defined(_DEBUG)
		void luaC_print_table(lua_State* L, int idx);

		void luaC_print_table_element(lua_State* L)
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
				{
					if (lua_isstring(L, -2))
					{
						const char* v_data = lua_tostring(L, -2);
						if (strcmp(v_data, "_G") == 0)
						{
							DebugOut("table");
							break;
						}
					}

					luaC_print_table(L, -2);
					break;
				}
			default:
				{
					//TODO: Check the name metafield instead of using this method
					SMUuid* v_uuid = reinterpret_cast<SMUuid*>(luaL_testudata(L, -1, "Uuid"));
					if (v_uuid)
					{
						DebugOut("{<Uuid>, ", v_uuid->ToString(), "}");
						break;
					}

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

		inline void luaC_print_table_internal(lua_State* L)
		{
			if (lua_isnumber(L, -2))
			{
				luaC_print_table_element(L);
			}
			else
			{
				DebugOut(lua_tostring(L, -2), " = ");
				luaC_print_table_element(L);
			}

			lua_pop(L, 1);
		}

		void luaC_print_table(lua_State* L, int idx)
		{
			lua_pushnil(L);

			DebugOut("{ ");

			if (lua_next(L, idx) != 0)
				luaC_print_table_internal(L);

			while (lua_next(L, idx) != 0)
			{
				DebugOut(", ");
				luaC_print_table_internal(L);
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
				luaC_print_table(L, idx);
				break;
			default:
				{
					SMUuid* v_uuid = reinterpret_cast<SMUuid*>(luaL_testudata(L, idx, "Uuid"));
					if (v_uuid)
					{
						DebugOut("{<Uuid>, ", v_uuid->ToString(), "}");
						break;
					}

					const int tt = luaL_getmetafield(L, idx, "__name"); //try name
					const char* kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) : luaL_typename(L, idx);
					lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, idx));
					if (tt != LUA_TNIL)
						lua_remove(L, -2); //remove __name

					DebugOut(lua_tostring(L, -1));
					lua_pop(L, 1);
					break;
				}
			}
		}
	#endif

		int Base::Print(lua_State* L)
		{
		#if defined(_DEBUG)
			const int n = lua_gettop(L); //number of arguments

			DebugOut("[LUA AHHHH] ");

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

		int Base::ToString(lua_State* L)
		{
			luaL_checkany(L, 1);
			luaL_tolstring(L, 1, NULL);

			return 1;
		}

		static const luaL_Reg g_base_functions[] =
		{
			{ "print"   , Lua::Base::Print    },
			{ "dofile"  , Lua::Base::Dofile   },
			{ "assert"  , Lua::Base::Assert   },
			{ "error"   , Lua::Base::Error    },
			{ "type"    , Lua::Base::Type     },
			{ "tostring", Lua::Base::ToString },

			{ LUA_GNAME , NULL },
			{ "_VERSION", NULL },
			{ NULL      , NULL }
		};

		void Base::Register(lua_State* L)
		{
			lua_pushglobaltable(L);
			luaL_setfuncs(L, g_base_functions, 0);

			lua_pushvalue(L, -1);
			lua_setfield(L, -2, LUA_GNAME);

			lua_pushliteral(L, LUA_VERSION);
			lua_setfield(L, -2, "_VERSION");
		}
	}
}