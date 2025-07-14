#pragma once
#include "RangeLimitedValue.h"

namespace
{
	constexpr float kHitPointMin = 0.0f;
	constexpr float kHitPointMax = 9999.0f;
}

using HitPoint_t = RangeLimitedValue<float, kHitPointMin, kHitPointMax>;