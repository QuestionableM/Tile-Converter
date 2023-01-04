#include "LuaColor.hpp"

#include "LuaBaseFunctions.hpp"
#include "LuaTableUtils.hpp"

#include "Utils\Color.hpp"
#include "Console.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

using SMColor = ::Color;

namespace SM
{
	namespace Lua
	{
		inline SMColor* CreateColorInternal(lua_State* L)
		{
			SMColor* v_color = reinterpret_cast<SMColor*>(lua_newuserdata(L, sizeof(SMColor)));
			luaL_setmetatable(L, "Color");

			return v_color;
		}

		int Color::New(lua_State* L)
		{
			const int v_arg_count = lua_gettop(L);
			if (v_arg_count == 0)
				return luaL_error(L, "Expected at least 1 argument (got: %d)", v_arg_count);

			if (v_arg_count == 1)
			{
				switch (lua_type(L, 1))
				{
				case LUA_TNUMBER:
					{
						const int v_col_hex_str = static_cast<int>(lua_tointeger(L, 1));
						SMColor* v_color = CreateColorInternal(L);
						v_color->SetIntBigEndian(v_col_hex_str);

						return 1;
					}
				case LUA_TSTRING:
					{
						const std::string v_color_str = lua_tostring(L, 1);
						SMColor* v_color = CreateColorInternal(L);
						(*v_color) = v_color_str;

						return 1;
					}
				default:
					G_LUA_CUSTOM_ARG_TYPE_ERROR(L, 1, LUA_TNUMBER);
				}
			}
			else if (v_arg_count == 3 || v_arg_count == 4)
			{
				G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
				G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
				G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

				if (v_arg_count == 4)
				{
					G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);

					SMColor* v_color = CreateColorInternal(L);
					v_color->SetRGBAFloat(
						lua_tonumber(L, 1),
						lua_tonumber(L, 2),
						lua_tonumber(L, 3),
						lua_tonumber(L, 4)
					);
				}
				else
				{
					SMColor* v_color = CreateColorInternal(L);
					v_color->SetRGBAFloat(
						lua_tonumber(L, 1),
						lua_tonumber(L, 2),
						lua_tonumber(L, 3),
						1.0
					);
				}

				return 1;
			}

			return luaL_error(L, "Expected 4 arguments (got %d)", v_arg_count);
		}

		int Color::ToString(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_color = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));

			lua_pushstring(L, v_color->StringHexAlpha().c_str());
			return 1;
		}

		int Color::Mul(lua_State* L) //can accept numbers
		{
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col1 = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			switch (Lua::Base::Type(L, 2))
			{
			case LUA_TNUMBER:
				{
					const double v_number = lua_tonumber(L, 2);

					SMColor* v_new_color = CreateColorInternal(L);
					v_new_color->SetRGBAFloat<double>(
						v_col1->GetFloat<double>(0) * v_number,
						v_col1->GetFloat<double>(1) * v_number,
						v_col1->GetFloat<double>(2) * v_number,
						v_col1->GetFloat<double>(3) * v_number
					);

					return 1;
				}
			case LUA_TSMCOLOR:
				{
					SMColor* v_col2 = reinterpret_cast<SMColor*>(lua_touserdata(L, 2));

					SMColor* v_new_color = CreateColorInternal(L);
					v_new_color->SetRGBAFloat<double>(
						v_col1->GetFloat<double>(0) * v_col2->GetFloat<double>(0),
						v_col1->GetFloat<double>(1) * v_col2->GetFloat<double>(1),
						v_col1->GetFloat<double>(2) * v_col2->GetFloat<double>(2),
						v_col1->GetFloat<double>(3) * v_col2->GetFloat<double>(3)
					);

					return 1;
				}
			}

			G_LUA_CUSTOM_ARG_TYPE_ERROR(L, 2, LUA_TSMCOLOR);
		}

		int Color::Div(lua_State* L) //can accept numbers
		{
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col1 = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			switch (Lua::Base::Type(L, 2))
			{
			case LUA_TNUMBER:
				{
					const double v_number = lua_tonumber(L, 2);

					SMColor* v_new_color = CreateColorInternal(L);
					v_new_color->SetRGBAFloat<double>(
						v_col1->GetFloat<double>(0) / v_number,
						v_col1->GetFloat<double>(1) / v_number,
						v_col1->GetFloat<double>(2) / v_number,
						v_col1->GetFloat<double>(3) / v_number
					);

					return 1;
				}
			case LUA_TSMCOLOR:
				{
					SMColor* v_col2 = reinterpret_cast<SMColor*>(lua_touserdata(L, 2));

					SMColor* v_new_color = CreateColorInternal(L);
					v_new_color->SetRGBAFloat<double>(
						v_col1->GetFloat<double>(0) / v_col2->GetFloat<double>(0),
						v_col1->GetFloat<double>(1) / v_col2->GetFloat<double>(1),
						v_col1->GetFloat<double>(2) / v_col2->GetFloat<double>(2),
						v_col1->GetFloat<double>(3) / v_col2->GetFloat<double>(3)
					);

					return 1;
				}
			}

			G_LUA_CUSTOM_ARG_TYPE_ERROR(L, 2, LUA_TSMCOLOR);
		}

		int Color::Add(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMCOLOR);

			SMColor* v_col1 = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			SMColor* v_col2 = reinterpret_cast<SMColor*>(lua_touserdata(L, 2));

			SMColor* v_new_color = CreateColorInternal(L);
			v_new_color->SetRGBAFloat<double>(
				v_col1->GetFloat<double>(0) + v_col2->GetFloat<double>(0),
				v_col1->GetFloat<double>(1) + v_col2->GetFloat<double>(1),
				v_col1->GetFloat<double>(2) + v_col2->GetFloat<double>(2),
				v_col1->GetFloat<double>(3) + v_col2->GetFloat<double>(3)
			);

			return 1;
		}

		int Color::Sub(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMCOLOR);

			SMColor* v_col1 = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			SMColor* v_col2 = reinterpret_cast<SMColor*>(lua_touserdata(L, 2));

			SMColor* v_new_color = CreateColorInternal(L);
			v_new_color->SetRGBAFloat<double>(
				v_col1->GetFloat<double>(0) - v_col2->GetFloat<double>(0),
				v_col1->GetFloat<double>(1) - v_col2->GetFloat<double>(1),
				v_col1->GetFloat<double>(2) - v_col2->GetFloat<double>(2),
				v_col1->GetFloat<double>(3) - v_col2->GetFloat<double>(3)
			);

			return 1;
		}

		int Color::Equals(lua_State* L)
		{
			SMColor* v_col1 = reinterpret_cast<SMColor*>(luaL_testudata(L, 1, "Color"));
			SMColor* v_col2 = reinterpret_cast<SMColor*>(luaL_testudata(L, 2, "Color"));

			if (v_col1 && v_col2)
			{
				lua_pushboolean(L, v_col1->m_color == v_col2->m_color);
				return 1;
			}

			lua_pushboolean(L, 0);
			return 1;
		}

		int Color::GetR(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(0));
			return 1;
		}

		int Color::GetG(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(1));
			return 1;
		}

		int Color::GetB(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(2));
			return 1;
		}

		int Color::GetA(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(3));
			return 1;
		}

		int Color::SetR(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			v_col->SetFloat(0, lua_tonumber(L, 2));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(0));
			return 1;
		}

		int Color::SetG(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			v_col->SetFloat(1, lua_tonumber(L, 2));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(1));
			return 1;
		}

		int Color::SetB(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			v_col->SetFloat(2, lua_tonumber(L, 2));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(2));
			return 1;
		}

		int Color::SetA(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMCOLOR);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			SMColor* v_col = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			v_col->SetFloat(3, lua_tonumber(L, 2));

			lua_pushnumber(L, v_col->GetFloat<lua_Number>(3));
			return 1;
		}

		int Color::NewIndex(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			SMColor* v_color = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			const char* v_index_str = lua_tostring(L, 2);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			switch (*v_index_str)
			{
			case 'r': v_color->SetFloat(0, lua_tonumber(L, 3)); return 0;
			case 'g': v_color->SetFloat(1, lua_tonumber(L, 3)); return 0;
			case 'b': v_color->SetFloat(2, lua_tonumber(L, 3)); return 0;
			case 'a': v_color->SetFloat(3, lua_tonumber(L, 3)); return 0;
			}

			return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);
		}

		int Color::Index(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);

			SMColor* v_color = reinterpret_cast<SMColor*>(lua_touserdata(L, 1));
			const char* v_index_str = lua_tostring(L, 2);

			if (strcmp(v_index_str, "getHexStr") == 0)
			{
				lua_pushcfunction(L, Color::ToString);
				return 1;
			}

			switch (*v_index_str)
			{
			case 'r': lua_pushnumber(L, v_color->GetFloat<lua_Number>(0)); return 1;
			case 'g': lua_pushnumber(L, v_color->GetFloat<lua_Number>(1)); return 1;
			case 'b': lua_pushnumber(L, v_color->GetFloat<lua_Number>(2)); return 1;
			case 'a': lua_pushnumber(L, v_color->GetFloat<lua_Number>(3)); return 1;
			}

			return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);
		}

		void Color::Register(lua_State* L)
		{
			{
				//Register the color table
				lua_pushstring(L, "color");
				lua_newtable(L);

				Table::PushFunction(L, "new", Color::New);

				Table::PushFunction(L, "getR", Color::GetR);
				Table::PushFunction(L, "getG", Color::GetG);
				Table::PushFunction(L, "getB", Color::GetB);
				Table::PushFunction(L, "getA", Color::GetA);

				Table::PushFunction(L, "setR", Color::SetR);
				Table::PushFunction(L, "setG", Color::SetG);
				Table::PushFunction(L, "setB", Color::SetB);
				Table::PushFunction(L, "setA", Color::SetA);

				lua_settable(L, -3);
			}

			{
				//Register the color metatable
				luaL_newmetatable(L, "Color");
				Table::PushPair(L, "__typeid", LUA_TSMCOLOR);
				Table::PushFunction(L, "__tostring", Color::ToString);
				Table::PushFunction(L, "__eq", Color::Equals);
				Table::PushFunction(L, "__add", Color::Add);
				Table::PushFunction(L, "__sub", Color::Sub);
				Table::PushFunction(L, "__mul", Color::Mul);
				Table::PushFunction(L, "__div", Color::Div);

				Table::PushFunction(L, "__newindex", Color::NewIndex);
				Table::PushFunction(L, "__index", Color::Index);

				lua_pop(L, 1);
			}
		}
	}
}