#pragma once
#include "main.h"
namespace Easing
{
	float EaseOutQuad(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}
	float EaseOutBounce(float t)
	{
		if (t < 4 / 11.0f)
		{
			return (121 * t * t) / 16.0f;
		}
		if (t < 8 / 11.0f)
		{
			return (363 / 40.0f * t * t) - (99 / 10.0f * t) + 17 / 5.0f;
		}
		if (t < 9 / 10.0f)
		{
			return (4356 / 361.0f * t * t) - (35442 / 1805.0f * t) + 16061 / 1805.0f;
		}
		return (54 / 5.0f * t * t) - (513 / 25.0f * t) + 268 / 25.0f;
	}

	float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}
	float EaseInCubic(float t)
	{
		return t * t * t;
	}
	float EaseOutCubic(float t)
	{
		return 1.0f - powf(1.0f - t, 3);
	}

	float EaseLerp(float current, float target, float speed)
	{
		float t = EaseOutCubic(speed);
		return current + (target - current) * t;
	}
	float SmoothStep(float t)
	{
		return t* t* (3 - 2 * t);
	}
};