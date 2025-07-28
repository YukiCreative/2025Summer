#pragma once
#include "RangeLimitedValue.h"
#include <unordered_map>
#include <list>

namespace StylishRankRange
{
	constexpr float kRankMin = 0.0f;
	constexpr float kRankMax = 100.0f;
}

// ���_�s��
enum class IncreaseStylishPointKind
{
	kSlashDown,
	kSlashLeftSide,
	kSlashRightSide,
	kSlashUp,
	kFinishAttack,
	kChargeAttack,
	kShockWaveAttack,
	kShockWave,
	kSpecialAttack,
};

// ���_�s��
enum class DecreaseStylishPointKind
{
	kDamage,
};

enum class StylishRankKind
{
	kNoRank, // ������ԁ@�����N�̕\���Ȃ�
	kDRank,
	kCRank,
	kBRank,
	kARank,
	kSRank,
	kSSRank,
	kSSSRank
};

struct StylishRankUIParam
{
	StylishRankKind m_rank = StylishRankKind::kNoRank;
	RangeLimitedValue<float, 0.0f, 1.0f> m_ratio;
};

class StylishRank
{
private:
	StylishRank();
	StylishRank(const StylishRank&) = delete;
	void operator=(const StylishRank&) = delete;

public:
	static StylishRank& GetInstance();

	void Init();

	void Update();

	// ���̓�͌v�Z�������Ⴄ�����Ȃ̂�Increase�Ƃ�Decrease�Ƃ��ɗ񋓂𕪂���K�v�͂Ȃ���������Ȃ�

	// �U�������������炱��ɍU����i��n��
	// ���̍U���̊�b�l��A�����ďo���ĂȂ����ȂǂŃ|�C���g��t����
	void IncreaseStylishPoint(const IncreaseStylishPointKind kind);
	// ���_�ƌv�Z�����قȂ�@��{�I�ɐݒ肳�ꂽ�l�̂܂܌��Z�����
	void DecreaseStylishPoint(const DecreaseStylishPointKind kind);

	// 0~100
	const float GetStylishPoint() const { return m_sp.Value(); }
	// ���̃X�^�C���b�V�������N�Ƃ��̊�����Ԃ�
	const StylishRankUIParam GetRankUIParam() const;

	void ResetRank();

private:

	using StylishPoint_t = RangeLimitedValue<float, StylishRankRange::kRankMin, StylishRankRange::kRankMax>;
	StylishPoint_t m_sp;

	std::unordered_map<IncreaseStylishPointKind, float> m_increasePointMap;
	std::unordered_map<DecreaseStylishPointKind, float> m_decreasePointMap;

	std::list<IncreaseStylishPointKind> m_stylishActionHistory;

	float m_spContinueDecreaseSpeed;

private:

	void InitMap();
};

