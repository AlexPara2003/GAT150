#pragma once

namespace neu
{
	constexpr float Pi = 3.14159265f; // 180
	constexpr float TwoPi = 6.2831853f; // 360
	constexpr float HalfPi = 1.57079633f; // 90

	constexpr float DegToRad(float degrees){ return degrees * (Pi / 180); }
	constexpr float RadToDeg(float radians){ return radians * (180 / Pi); }

	int sqr(int v);
	inline int half(int v) { return v / 2; }
}