#pragma once
#include <array>
#include <memory>

struct WaveResult;
enum class WaveRank;

namespace WaveConstants
{
	constexpr int kWaveNum = 5;

	using WaveResultArray_t = std::array<std::shared_ptr<WaveResult>, kWaveNum>;
	using WaveRankArray_t = std::array<WaveRank, kWaveNum>;
}
