#include "LuaUtil.hpp"

#include "LuaBaseFunctions.hpp"
#include "CLuaTableUtils.hpp"

#include "LuaVec3.hpp"
#include "LuaQuat.hpp"

#include <unordered_map>
#include <algorithm>
#include <cmath>


extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

#include <matrix.hpp>
#include <glm.hpp>

namespace SM
{
	namespace Lua
	{
		void Util::MatrixToQuaternion(float* a1, glm::quat* a2)
		{
			float v2; // xmm3_4
			float v3; // xmm2_4
			float v4; // xmm4_4
			float v6; // xmm1_4
			float v7; // xmm0_4
			float v8; // xmm4_4
			float v9; // xmm2_4
			float v10; // xmm3_4
			float v11; // xmm0_4
			__int64 v12; // r11
			__int64 v13; // r9
			__int64 v14; // rdx
			float v15; // xmm1_4
			//__int64 result; // rax
			float v17[3]; // [rsp+0h] [rbp-28h]
			float v18; // [rsp+Ch] [rbp-1Ch]

			v2 = *a1;
			v3 = a1[5];
			v4 = a1[10];
			v6 = (*a1 + v3) + v4;
			if (v6 <= 0.0)
			{
				v12 = 0i64;
				if (v2 >= v3)
				{
					if (v4 > v2)
						v12 = 2i64;
				}
				else
				{
					if (v4 > v3)
					{
						v12 &= 0xff00;
						v12 |= 1;
					}
					else
					{
						v12 &= 0xff00;
						v12 |= 0;
					}

					v12 = (unsigned int)(v12 + 1);
				}

				v13 = ((int)v12 + 1) % 3u;
				v14 = ((int)v12 + 2) % 3u;
				v15 = std::sqrtf(((a1[5 * v12] - a1[5 * v13]) - a1[5 * v14]) + 1.0f);
				v17[v12] = v15 * 0.5f;
				//*(float*)&v17[v12] = v15 * 0.5f;
				v18 = (a1[4 * v14 + v13] - a1[4 * v13 + v14]) * (0.5f / v15);
				//result = v14 + 4 * v12;
				//*(float*)&v17[v13] = (a1[4 * v13 + v12] + a1[4 * v12 + v13]) * (0.5f / v15);
				//*(float*)&v17[v14] = (a1[result] + a1[4 * v14 + v12]) * (0.5f / v15);
				v17[v13] = (a1[4 * v13 + v12] + a1[4 * v12 + v13]) * (0.5f / v15);
				v17[v14] = (a1[v14 + 4 * v12] + a1[4 * v14 + v12]) * (0.5f / v15);
				v8 = v18;
				v10 = v17[2]; //v10 = *(float*)&v17[2];
				v11 = v17[1]; //v11 = *(float*)&v17[1];
				v9 = v17[0]; //v9 = *(float*)v17;
			}
			else
			{
				v7 = std::sqrtf(v6 + 1.0f);
				v8 = v7 * 0.5f;
				v9 = (a1[9] - a1[6]) * (0.5f / v7);
				v10 = (a1[4] - a1[1]) * (0.5f / v7);
				v11 = (a1[2] - a1[8]) * (0.5f / v7);
			}

			a2->x = v9;
			a2->y = v11;
			a2->z = v10;
			a2->w = v8;
		}

		void Util::QuaternionToMatrix(float* a1, glm::quat* a2)
		{
			float v3; // xmm9_4
			float v4; // xmm10_4
			float v5; // xmm12_4
			float v6; // xmm13_4
			float v7; // xmm7_4
			float v8; // xmm4_4
			float v9; // xmm0_4
			float v10; // xmm0_4
			float v11; // xmm7_4
			float v12; // xmm8_4
			float v13; // xmm6_4
			float v14; // xmm5_4
			float v15; // xmm4_4
			float v16; // xmm12_4
			float v17; // xmm9_4
			float v18; // xmm13_4

			v3 = a2->w;//a2[3];
			v4 = a2->z;//a2[2];
			v5 = a2->x;//*a2;
			v6 = a2->y;//a2[1];
			v7 = a2->x;//*a2;
			v8 = a2->x;//*a2;

			v9 = 1.0f / ((v3 * v3 + v4 * v4) + (v5 * v5 + v6 * v6));
			v10 = v9 + v9;
			v11 = v7 * (v5 * v10);
			v12 = v3 * (v5 * v10);
			v13 = v3 * (v6 * v10);
			v14 = v6 * (v6 * v10);
			v15 = v8 * (v6 * v10);
			v16 = v5 * (v4 * v10);
			v17 = v3 * (v4 * v10);
			v18 = v6 * (v4 * v10);

			a1[0] = 1.0f - ((v4 * v4 * v10) + v14);
			a1[1] = v15 - v17;
			a1[2] = v16 + v13;
			a1[3] = 0.0f;

			a1[4] = v15 + v17;
			a1[5] = 1.0f - ((v4 * v4 * v10) + v11);
			a1[6] = v18 - v12;
			a1[7] = 0.0f;

			a1[8] = v16 - v13;
			a1[9] = v18 + v12;
			a1[10] = 1.0f - (v14 + v11);
			a1[11] = 0.0f;
		}


		int Util::Lerp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //first val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //second val
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //interpolation step

			const double v_first_val = lua_tonumber(L, 1);
			const double v_second_val = lua_tonumber(L, 2);
			const double v_step = lua_tonumber(L, 3);

			lua_pushnumber(L, Lua::Math::Lerp(v_first_val, v_second_val, v_step));
			return 1;
		}

		int Util::Clamp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER); //value
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER); //min
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER); //max

			const double v_value = lua_tonumber(L, 1);
			const double v_min = lua_tonumber(L, 2);
			const double v_max = lua_tonumber(L, 3);

			lua_pushnumber(L, std::clamp(v_value, v_min, v_max));
			return 1;
		}

		int Util::PositiveModulo(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			const long long v_int1 = static_cast<long long>(lua_tonumber(L, 1));
			const long long v_int2 = static_cast<long long>(lua_tonumber(L, 2));

			if (v_int2 == 0)
			{
				lua_pushinteger(L, 0);
				return 1;
			}

			lua_pushinteger(L, v_int1 % v_int2);
			return 1;
		}

		int Util::Smoothstep(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			const double v_edge0 = lua_tonumber(L, 1);
			const double v_edge1 = lua_tonumber(L, 2);
			const double v_x = lua_tonumber(L, 3);

			lua_pushnumber(L, glm::smoothstep(v_edge0, v_edge1, v_x));
			return 1;
		}

		int Util::Smootherstep(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			const double v_edge0 = lua_tonumber(L, 1);
			const double v_edge1 = lua_tonumber(L, 2);
			const double v_x = lua_tonumber(L, 3);

			lua_pushnumber(L, Lua::Math::Smootherstep(v_edge0, v_edge1, v_x));
			return 1;
		}

		int Util::Bezier2(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 4);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);

			const double v_c0 = lua_tonumber(L, 1);
			const double v_c1 = lua_tonumber(L, 2);
			const double v_c2 = lua_tonumber(L, 3);
			const double v_t = lua_tonumber(L, 4);

			lua_pushnumber(L, Lua::Math::QuadraticBezier(v_c0, v_c1, v_c2, v_t));
			return 1;
		}

		int Util::Bezier3(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 5);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 5, LUA_TNUMBER);

			const double v_c0 = lua_tonumber(L, 1);
			const double v_c1 = lua_tonumber(L, 2);
			const double v_c2 = lua_tonumber(L, 3);
			const double v_c3 = lua_tonumber(L, 4);
			const double v_t = lua_tonumber(L, 5);

			lua_pushnumber(L, Lua::Math::CubicBezier(v_c0, v_c1, v_c2, v_c3, v_t));
			return 1;
		}

		static const std::unordered_map<std::string, double (*)(const double&)> g_easingFunctions =
		{
			{ "linear"          , Easing::Linear           },
			{ "easeInSine"      , Easing::EaseInSine       },
			{ "easeOutSine"     , Easing::EaseOutSine      },
			{ "easeInOutSine"   , Easing::EaseInOutSine    },
			{ "easeInCubic"     , Easing::EaseInCubic      },
			{ "easeOutCubic"    , Easing::EaseOutCubic     },
			{ "easeInOutCubic"  , Easing::EaseInOutCubic   },
			{ "easeInQuint"     , Easing::EaseInQuint      },
			{ "easeOutQuint"    , Easing::EaseOutQuint     },
			{ "easeInOutQuint"  , Easing::EaseInOutQuint   },
			{ "easeInCirc"      , Easing::EaseInCirc       },
			{ "easeOutCirc"     , Easing::EaseOutCirc      },
			{ "easeInOutCirc"   , Easing::EaseInOutCirc    },
			{ "easeInElastic"   , Easing::EaseInElastic    },
			{ "easeOutElastic"  , Easing::EaseOutElastic   },
			{ "easeInOutElastic", Easing::EaseInOutElastic },
			{ "easeInQuad"      , Easing::EaseInQuad       },
			{ "easeOutQuad"     , Easing::EaseOutQuad      },
			{ "easeInOutQuad"   , Easing::EaseInOutQuad    },
			{ "easeInQuart"     , Easing::EaseInQuart      },
			{ "easeOutQuart"    , Easing::EaseOutQuart     },
			{ "easeInOutQuart"  , Easing::EaseInOutQuart   },
			{ "easeInExpo"      , Easing::EaseInExpo       },
			{ "easeOutExpo"     , Easing::EaseOutExpo      },
			{ "easeInOutExpo"   , Easing::EaseInOutExpo    },
			{ "easeInBack"      , Easing::EaseInBack       },
			{ "easeOutBack"     , Easing::EaseOutBack      },
			{ "easeInOutBack"   , Easing::EaseInOutBack    },
			{ "easeInBounce"    , Easing::EaseInBounce     },
			{ "easeOutBounce"   , Easing::EaseOutBounce    },
			{ "easeInOutBounce" , Easing::EaseInOutBounce  }
		};

		int Util::Easing(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSTRING);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			const std::string v_easing_type = lua_tostring(L, 1);
			const double v_x = lua_tonumber(L, 2);

			const auto v_iter = g_easingFunctions.find(v_easing_type);
			if (v_iter == g_easingFunctions.end())
			{
				lua_pushnumber(L, 0);
				return 1;
			}

			lua_pushnumber(L, v_iter->second(v_x));
			return 1;
		}

		int Util::AxesToQuat(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMVEC3);

			glm::vec3* v4 = LUA_VEC3_FROM_UDATA(L, 1);
			glm::vec3* v5 = LUA_VEC3_FROM_UDATA(L, 2);

			glm::mat4 v17;
			float* v_m_ptr = reinterpret_cast<float*>(&v17);
			v_m_ptr[0] = v4->x;
			v_m_ptr[1] = (v5->y * v4->z) - (v5->z * v4->y);
			v_m_ptr[2] = v5->x;
			v_m_ptr[3] = 0.0f;
			v_m_ptr[4] = v4->y;
			v_m_ptr[5] = (v5->z * v4->x) - (v5->x * v4->z);
			v_m_ptr[6] = v5->y;
			v_m_ptr[7] = 0.0f;
			v_m_ptr[8] = v4->z;
			v_m_ptr[9] = (v5->x * v4->y) - (v5->y * v4->x);
			v_m_ptr[10] = v5->z;
			v_m_ptr[11] = 0.0f;

			glm::quat* v_new_quat = Quat::CreateQuaternion(L);
			Util::MatrixToQuaternion(reinterpret_cast<float*>(&v17), v_new_quat);

			return 1;
		}

		void Util::Register(lua_State* L)
		{
			lua_pushstring(L, "util");
			lua_newtable(L);

			Table::PushFunction(L, "lerp", Util::Lerp);
			Table::PushFunction(L, "clamp", Util::Clamp);
			Table::PushFunction(L, "positiveModulo", Util::PositiveModulo);
			Table::PushFunction(L, "smoothstep", Util::Smoothstep);
			Table::PushFunction(L, "smootherstep", Util::Smootherstep);
			Table::PushFunction(L, "bezier2", Util::Bezier2);
			Table::PushFunction(L, "bezier3", Util::Bezier3);
			Table::PushFunction(L, "easing", Util::Easing);
			Table::PushFunction(L, "axesToQuat", Util::AxesToQuat);

			lua_settable(L, -3);
		}
	}
}