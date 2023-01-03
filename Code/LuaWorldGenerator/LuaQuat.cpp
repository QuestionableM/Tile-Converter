#include "LuaQuat.hpp"

#include "BaseLuaFunctions.hpp"
#include "CLuaTableUtils.hpp"
#include "LuaVec3.hpp"
#include "LuaUtil.hpp"
#include "Console.hpp"

extern "C"
{
	#include <lua\lua.h>
	#include <lua\lualib.h>
	#include <lua\lauxlib.h>
}

#include <gtx\quaternion.hpp>
#include <matrix.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>

#define LUA_QUAT_FROM_UDATA(L, I) reinterpret_cast<glm::quat*>(lua_touserdata(L, I))
#define LUA_QUAT_TEST_UDATA(L, I) reinterpret_cast<glm::quat*>(luaL_testudata(L, I, "Quat"))

namespace SM
{
	namespace Lua
	{
		glm::quat* Quat::CreateQuaternion(lua_State* L)
		{
			glm::quat* v_quat = reinterpret_cast<glm::quat*>(lua_newuserdata(L, sizeof(glm::quat)));
			luaL_setmetatable(L, "Quat");

			return v_quat;
		}

		int Quat::New(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 4);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 4, LUA_TNUMBER);

			glm::quat* v_quat = Quat::CreateQuaternion(L);
			v_quat->x = static_cast<float>(lua_tonumber(L, 1));
			v_quat->y = static_cast<float>(lua_tonumber(L, 2));
			v_quat->z = static_cast<float>(lua_tonumber(L, 3));
			v_quat->w = static_cast<float>(lua_tonumber(L, 4));

			return 1;
		}

		int Quat::Identity(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 0);

			glm::quat* v_quat = Quat::CreateQuaternion(L);
			v_quat->x = 0.0f;
			v_quat->y = 0.0f;
			v_quat->z = 0.0f;
			v_quat->w = 1.0f;

			return 1;
		}

		int Quat::GetX(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->x));
			return 1;
		}

		int Quat::GetY(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->y));
			return 1;
		}

		int Quat::GetZ(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->z));
			return 1;
		}

		int Quat::GetW(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->w));
			return 1;
		}

		int Quat::SetX(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			v_quat->x = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->x));
			return 1;
		}

		int Quat::SetY(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			v_quat->y = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->y));
			return 1;
		}

		int Quat::SetZ(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			v_quat->z = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->z));
			return 1;
		}

		int Quat::SetW(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TNUMBER);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			v_quat->w = static_cast<float>(lua_tonumber(L, 2));

			lua_pushnumber(L, static_cast<lua_Number>(v_quat->w));
			return 1;
		}

		int Quat::GetUp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			glm::vec3* v_new_vec = Vec3::CreateVector3(L);
			(*v_new_vec) = glm::rotate(*v_quat, glm::vec3(0.0f, 1.0f, 0.0f));

			return 1;
		}

		int Quat::GetRight(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			glm::vec3* v_new_vec = Vec3::CreateVector3(L);
			(*v_new_vec) = glm::rotate(*v_quat, glm::vec3(1.0f, 0.0f, 0.0f));

			return 1;
		}

		int Quat::GetAt(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			glm::vec3* v_new_vec = Vec3::CreateVector3(L);
			(*v_new_vec) = glm::rotate(*v_quat, glm::vec3(0.0f, 0.0f, 1.0f));

			return 1;
		}

		int Quat::AngleAxis(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TNUMBER);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMVEC3);

			const float v_angle = static_cast<float>(lua_tonumber(L, 1));
			glm::vec3* v_axis = LUA_VEC3_FROM_UDATA(L, 2);

			glm::quat* v_quat = Quat::CreateQuaternion(L);
			(*v_quat) = glm::angleAxis(v_angle, *v_axis);

			return 1;
		}

		int Quat::Inverse(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			glm::quat* v_new_quat = Quat::CreateQuaternion(L);
			(*v_new_quat) = glm::inverse(*v_quat);

			return 1;
		}

		int Quat::FromEuler(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);

			glm::vec3* v_euler = LUA_VEC3_FROM_UDATA(L, 1);
			(*v_euler) = glm::radians(*v_euler);

			glm::mat4 v_matrix = glm::eulerAngleXYZ(v_euler->x, v_euler->y, v_euler->z);

			glm::quat* v_new_quat = Quat::CreateQuaternion(L);
			(*v_new_quat) = glm::quat_cast(v_matrix);

			return 1;
		}

		int Quat::Slerp(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMQUAT);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			glm::quat* v_quat1 = LUA_QUAT_FROM_UDATA(L, 1);
			glm::quat* v_quat2 = LUA_QUAT_FROM_UDATA(L, 2);
			const float v_step = static_cast<float>(lua_tonumber(L, 3));

			glm::quat* v_output = Quat::CreateQuaternion(L);
			(*v_output) = glm::slerp(*v_quat1, *v_quat2, v_step);

			return 1;
		}

		int Quat::LookRotation(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMVEC3);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 2, LUA_TSMVEC3);

			glm::vec3* v_vec1 = LUA_VEC3_FROM_UDATA(L, 1);
			glm::vec3* v_vec2 = LUA_VEC3_FROM_UDATA(L, 2);

			glm::quat* v_output = Quat::CreateQuaternion(L);
			(*v_output) = glm::quat_cast(glm::lookAt(glm::vec3(0), *v_vec1, *v_vec2));

			return 1;
		}

		inline void lua_euler_angles_round90(glm::vec3& vec)
		{
			/* just a little reminder
				pitch - x
				yaw - y
				roll - z
			*/
			if (std::fabsf(vec.x) == glm::half_pi<float>())
			{
				if (vec.z <= 0.0f)
					vec.z += glm::pi<float>();
				else
					vec.z -= glm::pi<float>();

				if (vec.y <= 0.0f)
					vec.y += glm::pi<float>();
				else
					vec.y -= glm::pi<float>();
			}

			vec.x = std::roundf(vec.x * glm::two_over_pi<float>()) * glm::half_pi<float>();
			vec.y = std::roundf(vec.y * glm::two_over_pi<float>()) * glm::half_pi<float>();
			vec.z = std::roundf(vec.z * glm::two_over_pi<float>()) * glm::half_pi<float>();
		}

		int Quat::Round90(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 1);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);

			glm::vec3 v_euler_angles = glm::eulerAngles(*v_quat);
			lua_euler_angles_round90(v_euler_angles);

			glm::quat* v_new_quat = Quat::CreateQuaternion(L);
			(*v_new_quat) = glm::quat(v_euler_angles);

			return 1;
		}

		int Quat::Mul(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);
			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 1, LUA_TSMQUAT);

			glm::quat* v_quat1 = LUA_QUAT_FROM_UDATA(L, 1);

			switch (Lua::Base::Type(L, 2))
			{
			case LUA_TSMQUAT:
				{
					glm::quat* v_quat2 = LUA_QUAT_FROM_UDATA(L, 2);

					glm::quat* v_output = Quat::CreateQuaternion(L);
					(*v_output) = (*v_quat1) * (*v_quat2);

					return 1;
				}
			case LUA_TSMVEC3:
				{
					glm::vec3* v_vec = LUA_VEC3_FROM_UDATA(L, 2);

					glm::vec3* v_output = Vec3::CreateVector3(L);
					(*v_output) = (*v_quat1) * (*v_vec);

					return 1;
				}
			}

			G_LUA_CUSTOM_ARG_TYPE_ERROR(L, 2, LUA_TSMQUAT);
		}

		int Quat::Equals(lua_State* L)
		{
			glm::quat* v_quat1 = LUA_QUAT_TEST_UDATA(L, 1);
			glm::quat* v_quat2 = LUA_QUAT_TEST_UDATA(L, 2);

			if (v_quat1 && v_quat2)
			{
				lua_pushboolean(L, (*v_quat1) == (*v_quat2));
				return 1;
			}

			lua_pushboolean(L, 0);
			return 1;
		}

		int Quat::Index(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 2);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			const char* v_index_str = lua_tostring(L, 2);

			switch (*v_index_str)
			{
			case 'x': lua_pushnumber(L, static_cast<lua_Number>(v_quat->x)); return 1;
			case 'y': lua_pushnumber(L, static_cast<lua_Number>(v_quat->y)); return 1;
			case 'z': lua_pushnumber(L, static_cast<lua_Number>(v_quat->z)); return 1;
			case 'w': lua_pushnumber(L, static_cast<lua_Number>(v_quat->w)); return 1;
			}

			return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);
		}

		int Quat::NewIndex(lua_State* L)
		{
			G_LUA_CUSTOM_ARG_CHECK(L, 3);

			glm::quat* v_quat = LUA_QUAT_FROM_UDATA(L, 1);
			const char* v_index_str = lua_tostring(L, 2);

			G_LUA_CUSTOM_ARG_TYPE_CHECK(L, 3, LUA_TNUMBER);

			switch (*v_index_str)
			{
			case 'x': v_quat->x = static_cast<float>(lua_tonumber(L, 3)); return 0;
			case 'y': v_quat->y = static_cast<float>(lua_tonumber(L, 3)); return 0;
			case 'z': v_quat->z = static_cast<float>(lua_tonumber(L, 3)); return 0;
			case 'w': v_quat->w = static_cast<float>(lua_tonumber(L, 3)); return 0;
			}

			return luaL_error(L, "Unknown member '%s' in userdata", v_index_str);
		}

		void Quat::Register(lua_State* L)
		{
			{
				lua_pushstring(L, "quat");
				lua_newtable(L);

				//Create quaternion functions
				Table::PushFunction(L, "new", Quat::New);
				Table::PushFunction(L, "identity", Quat::Identity);

				Table::PushFunction(L, "angleAxis", Quat::AngleAxis);
				Table::PushFunction(L, "inverse", Quat::Inverse);
				Table::PushFunction(L, "fromEuler", Quat::FromEuler);
				Table::PushFunction(L, "slerp", Quat::Slerp);
				Table::PushFunction(L, "lookRotation", Quat::LookRotation);
				Table::PushFunction(L, "round90", Quat::Round90);

				Table::PushFunction(L, "getRight", Quat::GetRight);
				Table::PushFunction(L, "getAt", Quat::GetAt);
				Table::PushFunction(L, "getUp", Quat::GetUp);

				Table::PushFunction(L, "getX", Quat::GetX);
				Table::PushFunction(L, "getY", Quat::GetY);
				Table::PushFunction(L, "getZ", Quat::GetZ);
				Table::PushFunction(L, "getW", Quat::GetW);

				Table::PushFunction(L, "setX", Quat::SetX);
				Table::PushFunction(L, "setY", Quat::SetY);
				Table::PushFunction(L, "setZ", Quat::SetZ);
				Table::PushFunction(L, "setW", Quat::SetW);

				lua_settable(L, -3);
			}

			{
				//Create Quat metatable
				luaL_newmetatable(L, "Quat");
				Table::PushPair(L, "__typeid", LUA_TSMQUAT);
				Table::PushFunction(L, "__mul", Quat::Mul);
				Table::PushFunction(L, "__eq", Quat::Equals);
				Table::PushFunction(L, "__newindex", Quat::NewIndex);
				Table::PushFunction(L, "__index", Quat::Index);

				lua_pop(L, 1);
			}
		}
	}
}