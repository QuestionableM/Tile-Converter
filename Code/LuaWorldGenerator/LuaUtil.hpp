#pragma once

extern "C"
{
	#include <lua\lstate.h>
}

#include <cmath>

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

		namespace Easing
		{
			inline constexpr const static double PI_VALUE = 3.14159265358979323846264338327950288;

			inline double Linear(const double& x)
			{
				return x;
			}

			inline double EaseInSine(const double& x)
			{
				return 1.0 - std::cos((x * PI_VALUE) / 2.0);
			}

			inline double EaseOutSine(const double& x)
			{
				return std::sin((x * PI_VALUE) / 2.0);
			}

			inline double EaseInOutSine(const double& x)
			{
				return -(std::cos(PI_VALUE * x) - 1.0) / 2.0;
			}

			inline double EaseInCubic(const double& x)
			{
				return x * x * x;
			}

			inline double EaseOutCubic(const double& x)
			{
				return 1.0 - std::pow(1.0 - x, 3.0);
			}

			inline double EaseInOutCubic(const double& x)
			{
				return (x < 0.5)
					? (4.0 * x * x * x)
					: (1.0 - std::pow(-2.0 * x + 2.0, 3.0) / 2.0);
			}

			inline double EaseInQuint(const double& x)
			{
				return x * x * x * x * x;
			}

			inline double EaseOutQuint(const double& x)
			{
				return 1.0 - std::pow(1.0 - x, 5.0);
			}

			inline double EaseInOutQuint(const double& x)
			{
				return (x < 0.5)
					? (16.0 * x * x * x * x * x)
					: (1.0 - std::pow(-2.0 * x + 2.0, 5.0) / 2.0);
			}

			inline double EaseInCirc(const double& x)
			{
				return 1.0 - std::sqrt(1.0 - (x * x));
			}

			inline double EaseOutCirc(const double& x)
			{
				return 1.0 - std::pow(x - 1.0, 2.0);
			}

			inline double EaseInOutCirc(const double& x)
			{
				return (x < 0.5)
					? ((1.0 - std::sqrt(1.0 - std::pow(2.0 * x, 2.0))) / 2.0)
					: ((std::sqrt(1.0 - std::pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0);
			}

			inline double EaseInElastic(const double& x)
			{
				constexpr const double c4 = (2.0 * PI_VALUE) / 3.0;
				return (x == 0) ? 0.0 : (x == 1) ? 1.0 : (-std::pow(2.0, 10.0 * x - 10.0) * std::sin((x * 10.0 - 10.75) * c4));
			}

			inline double EaseOutElastic(const double& x)
			{
				constexpr const double c4 = (2.0 * PI_VALUE) / 3.0;
				return (x == 0.0) ? 0.0 : (x == 1.0) ? 1.0 : (std::pow(2.0, -10.0 * x) * std::sin((x * 10.0 - 0.75) * c4));
			}

			inline double EaseInOutElastic(const double& x)
			{
				constexpr const double c5 = (2.0 * PI_VALUE) / 4.5;
				return (x == 0.0) ? 0.0 : (x == 1.0) ? 1.0 : (x < 0.5)
					? (-(std::pow(2.0, 20.0 * x - 10.0) * std::sin((20.0 * x - 11.125) * c5)) / 2.0)
					: ((std::pow(2.0, -20.0 * x + 10.0) * std::sin((20.0 * x - 11.125) * c5)) / 2.0 + 1.0);
			}

			inline double EaseInQuad(const double& x)
			{
				return x * x;
			}

			inline double EaseOutQuad(const double& x)
			{
				return 1.0 - (1.0 - x) * (1.0 - x);
			}

			inline double EaseInOutQuad(const double& x)
			{
				return (x < 0.5)
					? (2.0 * x * x)
					: (1.0 - std::pow(-2.0 * x + 2.0, 2.0) / 2.0);
			}

			inline double EaseInQuart(const double& x)
			{
				return x * x * x * x;
			}

			inline double EaseOutQuart(const double& x)
			{
				return 1.0 - std::pow(1.0 - x, 4.0);
			}

			inline double EaseInOutQuart(const double& x)
			{
				return (x < 0.5)
					? (8.0 * x * x * x * x)
					: (1.0 - std::pow(-2.0 * x + 2.0, 4.0) / 2.0);
			}

			inline double EaseInExpo(const double& x)
			{
				return (x == 0.0) ? 0.0 : std::pow(2.0, 10.0 * x - 10.0);
			}

			inline double EaseOutExpo(const double& x)
			{
				return (x == 1.0) ? 1.0 : (1.0 - std::pow(2.0, -10.0 * x));
			}

			inline double EaseInOutExpo(const double& x)
			{
				return (x == 0.0) ? 0.0 : (x == 1.0) ? 1.0 : (x < 0.5)
					? (std::pow(2.0, 20.0 * x - 10.0) / 2.0)
					: (2.0 - std::pow(2.0, -20.0 * x + 10.0) / 2.0);
			}

			inline double EaseInBack(const double& x)
			{
				constexpr const double c1 = 1.70158;
				constexpr const double c3 = c1 + 1.0;

				return c3 * x * x * x - c1 * x * x;
			}

			inline double EaseOutBack(const double& x)
			{
				constexpr const double c1 = 1.70158;
				constexpr const double c3 = c1 + 1.0;

				return 1.0 + c3 * std::pow(x - 1.0, 3.0) + c1 * std::pow(x - 1.0, 2.0);
			}

			inline double EaseInOutBack(const double& x)
			{
				constexpr const double c1 = 1.70158;
				constexpr const double c2 = c1 * 1.525;

				return (x < 0.5)
					? ((std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0)
					: ((std::pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) + 2.0) / 2.0);
			}

			inline double EaseOutBounce(const double& x_input)
			{
				double x = x_input;

				constexpr const double n1 = 7.5625;
				constexpr const double d1 = 2.75;

				if (x < 1.0 / d1) {
					return n1 * x * x;
				}
				else if (x < 2.0 / d1) {
					return n1 * (x -= 1.5 / d1) * x + 0.75;
				}
				else if (x < 2.5 / d1) {
					return n1 * (x -= 2.25 / d1) * x + 0.9375;
				}
				else {
					return n1 * (x -= 2.625 / d1) * x + 0.984375;
				}
			}

			inline double EaseInBounce(const double& x)
			{
				return 1.0 - Easing::EaseOutBounce(1.0 - x);
			}

			inline double EaseInOutBounce(const double& x)
			{
				return (x < 0.5)
					? (1.0 - Easing::EaseOutBounce(1.0 - 2.0 * x) / 2.0)
					: (1.0 + Easing::EaseOutBounce(2.0 * x - 1.0) / 2.0);
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
			int Easing(lua_State* L);
			int AxesToQuat(lua_State* L);

			void Register(lua_State* L);
		}
	}
}