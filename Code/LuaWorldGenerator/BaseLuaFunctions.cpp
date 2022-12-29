#include "BaseLuaFunctions.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Console.hpp"

#include <filesystem>
#include <string>

extern "C"
{
	#include <lua\lauxlib.h>
	#include <lua\luaconf.h>
	#include <lua\lualib.h>
	#include <lua\lapi.h>
	#include <lua\lua.h>
}

#include <glm.hpp>

//A link to the bit library
extern "C" int luaopen_bit(lua_State * L);

namespace fs = std::filesystem;

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

			const std::wstring v_old_file = Base::GetCurrentFile();
			if (!File::Exists(v_file_path))
			{
				const fs::path v_path = v_old_file;
				if (v_path.has_parent_path())
					v_file_path = v_path.parent_path().wstring() + L"/" + v_file_path;
			}

			DebugOutL("Dofile: ", v_file_path);

			lua_settop(L, 1);

			if (luaL_loadfile(L, String::ToUtf8(v_file_path).c_str()) != LUA_OK)
				return lua_error(L);

			{
				Base::SetCurrentFile(v_file_path);
				lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
				Base::SetCurrentFile(v_old_file);
			}

			return dofilecont(L, 0, 0);
		}

		int Base::TypeLua(lua_State* L)
		{
			const int t = lua_type(L, 1);
			luaL_argcheck(L, t != LUA_TNONE, 1, "value expected");

			const int v_tt = luaL_getmetafield(L, 1, "__typeid");
			if (v_tt == LUA_TNUMBER)
			{
				const int v_type_idx = static_cast<int>(lua_tointeger(L, -1));

				lua_pop(L, 1); //pop __typeid off the stack

				lua_pushstring(L, Base::Typename(v_type_idx));
				return 1;
			}

			if (v_tt != LUA_TNIL)
				lua_remove(L, -1);

			lua_pushstring(L, luaL_typename(L, 1));
			return 1;
		}

		inline int luaC_print_type_get_udata_idx(int idx)
		{
			return (idx == -1) ? -2 : idx;
		}

		inline void luaC_print_type(lua_State* L, int idx)
		{
			const int v_type_idx = Base::Type(L, idx);
			switch (v_type_idx)
			{
			case LUA_TSMUUID:
				{
					SMUuid* v_uuid = reinterpret_cast<SMUuid*>(lua_touserdata(L, luaC_print_type_get_udata_idx(idx)));
					DebugOut("{<Uuid>, ", v_uuid->ToString(), "}");
					break;
				}
			case LUA_TSMVEC3:
				{
					glm::vec3* v_vec3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, luaC_print_type_get_udata_idx(idx)));
					DebugOut("{<Vec3>, ", v_vec3->x, ", ", v_vec3->y, ", ", v_vec3->z, "}");
					break;
				}
			default:
				DebugOut(Base::Typename(v_type_idx));
				break;
			}
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
					if (lua_type(L, -2) == LUA_TSTRING)
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
				luaC_print_type(L, -1);
				break;
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

			DebugOut("{");

			if (lua_next(L, idx) != 0)
			{
				luaC_print_table_internal(L);

				while (lua_next(L, idx) != 0)
				{
					DebugOut(", ");
					luaC_print_table_internal(L);
				}
			}

			DebugOut("}");
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
				luaC_print_type(L, idx);
				break;
			}
		}
	#endif

		int Base::Print(lua_State* L)
		{
		#if defined(_DEBUG)
			const int n = lua_gettop(L); //number of arguments

			DebugOut("[LUA] ");

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

		#define SPACECHARS	" \f\n\r\t\v"

		static const char* b_str2int(const char* s, int base, lua_Integer* pn) {
			lua_Unsigned n = 0;
			int neg = 0;
			s += strspn(s, SPACECHARS);  /* skip initial spaces */
			if (*s == '-') { s++; neg = 1; }  /* handle sign */
			else if (*s == '+') s++;
			if (!isalnum((unsigned char)*s))  /* no digit? */
				return NULL;
			do {
				int digit = (isdigit((unsigned char)*s)) ? *s - '0'
					: (toupper((unsigned char)*s) - 'A') + 10;
				if (digit >= base) return NULL;  /* invalid numeral */
				n = n * base + digit;
				s++;
			} while (isalnum((unsigned char)*s));
			s += strspn(s, SPACECHARS);  /* skip trailing spaces */
			*pn = (lua_Integer)((neg) ? (0u - n) : n);
			return s;
		}

		int Base::ToNumber(lua_State* L)
		{
			if (lua_isnoneornil(L, 2)) {  /* standard conversion? */
				if (lua_type(L, 1) == LUA_TNUMBER) {  /* already a number? */
					lua_settop(L, 1);  /* yes; return it */
					return 1;
				}
				else {
					size_t l;
					const char* s = lua_tolstring(L, 1, &l);
					if (s != NULL && lua_stringtonumber(L, s) == l + 1)
						return 1;  /* successful conversion to number */
					/* else not a number */
					luaL_checkany(L, 1);  /* (but there must be some parameter) */
				}
			}
			else {
				size_t l;
				const char* s;
				lua_Integer n = 0;  /* to avoid warnings */
				lua_Integer base = luaL_checkinteger(L, 2);
				luaL_checktype(L, 1, LUA_TSTRING);  /* no numbers as strings */
				s = lua_tolstring(L, 1, &l);
				luaL_argcheck(L, 2 <= base && base <= 36, 2, "base out of range");
				if (b_str2int(s, (int)base, &n) == s + l) {
					lua_pushinteger(L, n);
					return 1;
				}  /* else not a number */
			}  /* else not a number */
			luaL_pushfail(L);  /* not a number */
			return 1;
		}

		int Base::Type(lua_State* L, const int& idx)
		{
			const int v_tt = luaL_getmetafield(L, idx, "__typeid");
			if (v_tt == LUA_TNUMBER)
			{
				const int v_type_idx = static_cast<int>(lua_tointeger(L, -1));

				lua_pop(L, 1); //pop __typeid off the stack

				return v_type_idx;
			}

			if (v_tt != LUA_TNIL)
				lua_pop(L, 1); //pop __typeid off the stack

			return lua_type(L, idx);
		}

		const char* Base::Typename(const int& idx)
		{
			switch (idx)
			{
			//Base lua types
			case LUA_TNIL: return "nil";
			case LUA_TBOOLEAN: return "boolean";
			case LUA_TLIGHTUSERDATA: return "userdata";
			case LUA_TNUMBER: return "number";
			case LUA_TSTRING: return "string";
			case LUA_TTABLE: return "table";
			case LUA_TFUNCTION: return "function";
			case LUA_TUSERDATA: return "userdata";
			case LUA_TTHREAD: return "thread";

			//Custom lua types
			case LUA_TSMVEC3: return "Vec3";
			case LUA_TSMQUAT: return "Quat";
			case LUA_TSMUUID: return "Uuid";

			default: return "unknown";
			}
		}

		static const luaL_Reg g_base_functions[] =
		{
			{ "print"   , Lua::Base::Print    },
			{ "dofile"  , Lua::Base::Dofile   },
			{ "assert"  , Lua::Base::Assert   },
			{ "error"   , Lua::Base::Error    },
			{ "type"    , Lua::Base::TypeLua  },
			{ "tostring", Lua::Base::ToString },
			{ "tonumber", Lua::Base::ToNumber },

			{ LUA_GNAME , NULL },
			{ "_VERSION", NULL },
			{ NULL      , NULL }
		};

		/*
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
	*/

		void Base::Register(lua_State* L)
		{
			{
				//register the base functions
				lua_pushglobaltable(L);
				luaL_setfuncs(L, g_base_functions, 0);

				lua_pushvalue(L, -1);
				lua_setfield(L, -2, LUA_GNAME);

				lua_pushliteral(L, LUA_VERSION);
				lua_setfield(L, -2, "_VERSION");
			}

			//register bit library
			luaL_requiref(L, "bit", luaopen_bit, 1);
			lua_pop(L, 1);

			//register math library
			luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
			lua_pop(L, 1);

			//register string library
			luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
			lua_pop(L, 1);

			//register table library
			luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
			lua_pop(L, 1);
		}
	}
}