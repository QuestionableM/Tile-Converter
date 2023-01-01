#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

namespace SM
{
	namespace Lua
	{
		namespace Math
		{
			template<typename T>
			inline constexpr T Lerp(const T& a, const T& b, const T& f)
			{
				static_assert(std::is_floating_point_v<T>, "Lerp can only be used with floating point values");

				return a * (T(1.0) - f) + (b * f);
			}

			template<typename T>
			inline constexpr T Smootherstep(const T& edge0, const T& edge1, T x)
			{
				static_assert(std::is_floating_point_v<T>, "Smootherstep can only be used with floating point values");

				x = std::clamp((x - edge0) / (edge1 - edge0), T(0.0), T(1.0));

				return x * x * x * (x * (x * T(6) - T(15)) + T(10));
			}

			template<typename T>
			inline constexpr T QuadraticBezier(const T& a, const T& b, const T& c, const T& t)
			{
				static_assert(std::is_floating_point_v<T>, "QuadraticBezier can only be used with floating point values");

				const T t_inv = T(1.0) - t;

				return a * (t_inv * t_inv) + b * 2 * t_inv * t + c * (t * t);
			}

			template<typename T>
			inline constexpr T CubicBezier(const T& a, const T& b, const T& c, const T& d, const T& t)
			{
				static_assert(std::is_floating_point_v<T>, "CubicBezier can only be used with floating point values");

				const T t_inv = T(1.0) - t;

				return a * (t_inv * t_inv * t_inv) + 3 * b * (t_inv * t_inv) * t + 3 * c * t_inv * (t * t) + d * (t * t * t);
			}
		}

		namespace Util
		{
			int Lerp(lua_State* L);
			int Clamp(lua_State* L);
			int PositiveModulo(lua_State* L);
			int Smoothstep(lua_State* L);
			int Smootherstep(lua_State* L);
			int Bezier2(lua_State* L);
			int Bezier3(lua_State* L);

			void Register(lua_State* L);
		}
	}
}