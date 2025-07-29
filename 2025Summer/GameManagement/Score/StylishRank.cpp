#include "StylishRank.h"
#include "Game.h"
#include <cassert>

namespace
{
	constexpr size_t kMaxHistory = 10;

	// �ʒu�t���[���Ɏ��R��������ő呬�x
	constexpr float kMaxDecreaseSpeed = 5.0f / Game::kFrameRate;
	// ���R���������x
	constexpr float kContinueDecreaseAccel = 0.0003f;

	// �����܂ŒB�����炻�̃����N
	constexpr float kDRankThreshold = 10.0f;
	constexpr float kCRankThreshold = 20.0f;
	constexpr float kBRankThreshold = 30.0f;
	constexpr float kARankThreshold = 40.0f;
	constexpr float kSRankThreshold = 50.0f;
	constexpr float kSSRankThreshold = 70.0f;
	constexpr float kSSSRankThreshold = 90.0f;
}

StylishRank& StylishRank::GetInstance()
{
	static StylishRank instance;
	return instance;
}

StylishRank::StylishRank() :
	m_spContinueDecreaseSpeed(0.0f)
{
}

void StylishRank::Init()
{
	InitMap();
}

void StylishRank::Update()
{
	// ���ԂŃX�^�C���b�V���|�C���g������������
	// �������Ă��Ȃ����Ԃ������قǌ����ʂ𑝂₷
	// ���������玞�ԃ��Z�b�g
	m_sp -= m_spContinueDecreaseSpeed;

	m_spContinueDecreaseSpeed += kContinueDecreaseAccel;
	if (m_spContinueDecreaseSpeed > kMaxDecreaseSpeed) m_spContinueDecreaseSpeed = kMaxDecreaseSpeed;
}

void StylishRank::IncreaseStylishPoint(const IncreaseStylishPointKind kind)
{
	assert(m_increasePointMap.contains(kind) && "�Ή����Ă�񋓑̗̂v�f��Map�ɂȂ�������");

	// �A�����ē������@�ŉ��_����Ă�����_�ʂ����炷
	const int historyCount = std::count(m_stylishActionHistory.begin(), m_stylishActionHistory.end(), kind);

	// ���̌v�Z���Ȃ���ڂ�100�����ڂ�50���O��ڂ�33���c�ɂȂ��Ă���
	// ��ŕς��邩��
	m_sp += m_increasePointMap[kind] / (historyCount + 1);

	if (m_stylishActionHistory.size() > kMaxHistory)
	{
		m_stylishActionHistory.pop_front();
	}
	m_stylishActionHistory.emplace_back(kind);

	// �X�^�C���b�V���Q�[�W���R�������x�����Z�b�g
	m_spContinueDecreaseSpeed = 0.0f;
}

void StylishRank::DecreaseStylishPoint(const DecreaseStylishPointKind kind)
{
	assert(m_decreasePointMap.contains(kind) && "�Ή����Ă�񋓑̗̂v�f��Map�ɂȂ�������");
	m_sp -= m_decreasePointMap[kind];
}

const StylishRankUIParam StylishRank::GetRankUIParam() const
{
	StylishRankUIParam result;

	// 0~100�̂����A�ǂꂭ�炢����D~SSS�܂ŕ�����
	if		(m_sp.Value() < kDRankThreshold)	result.m_rank = StylishRankKind::kNoRank;
	else if (m_sp.Value() < kCRankThreshold)	result.m_rank = StylishRankKind::kDRank;
	else if (m_sp.Value() < kBRankThreshold)	result.m_rank = StylishRankKind::kCRank;
	else if (m_sp.Value() < kARankThreshold)	result.m_rank = StylishRankKind::kBRank;
	else if (m_sp.Value() < kSRankThreshold)	result.m_rank = StylishRankKind::kARank;
	else if (m_sp.Value() < kSSRankThreshold)	result.m_rank = StylishRankKind::kSRank;
	else if (m_sp.Value() < kSSSRankThreshold)	result.m_rank = StylishRankKind::kSSRank;
	else										result.m_rank = StylishRankKind::kSSSRank;

	// ���̃����N���玟�̃����N(�������͏��)�܂łǂꂭ�炢�̊��������o���ĕԂ�
	switch (result.m_rank)
	{
	case StylishRankKind::kNoRank:
		result.m_ratio = m_sp.Value() / kDRankThreshold;
		break;
	case StylishRankKind::kDRank:
		result.m_ratio = (m_sp.Value() - kDRankThreshold) / (kCRankThreshold - kDRankThreshold);
		break;
	case StylishRankKind::kCRank:
		result.m_ratio = (m_sp.Value() - kCRankThreshold) / (kBRankThreshold - kCRankThreshold);
		break;
	case StylishRankKind::kBRank:
		result.m_ratio = (m_sp.Value() - kBRankThreshold) / (kARankThreshold - kBRankThreshold);
		break;
	case StylishRankKind::kARank:
		result.m_ratio = (m_sp.Value() - kARankThreshold) / (kSRankThreshold - kARankThreshold);
		break;
	case StylishRankKind::kSRank:
		result.m_ratio = (m_sp.Value() - kSRankThreshold) / (kSSRankThreshold - kSRankThreshold);
		break;
	case StylishRankKind::kSSRank:
		result.m_ratio = (m_sp.Value() - kSSRankThreshold) / (kSSSRankThreshold - kSSRankThreshold);
		break;
	case StylishRankKind::kSSSRank:
		result.m_ratio = (m_sp.Value() - kSSSRankThreshold) / (StylishRankRange::kRankMax - kSSSRankThreshold);
		break;
	default:
		assert(false && "StylishKind��switch���R��");
		break;
	}

	return result;
}

void StylishRank::ResetRank()
{
	m_sp.SetMin();
	m_stylishActionHistory.clear();
}

void StylishRank::InitMap()
{
	// �ЂƂ܂��萔
	// ���ƂŊO�������邩��
	m_increasePointMap[IncreaseStylishPointKind::kSlashDown] = 2.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashLeftSide] = 2.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashRightSide] = 5.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashUp] = 5.0f;
	m_increasePointMap[IncreaseStylishPointKind::kFinishAttack] = 15.0f;
	m_increasePointMap[IncreaseStylishPointKind::kChargeAttack] = 8.0f;
	m_increasePointMap[IncreaseStylishPointKind::kShockWaveAttack] = 3.0f;
	m_increasePointMap[IncreaseStylishPointKind::kShockWave] = 8.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSpecialAttack] = 20.0f;

	m_decreasePointMap[DecreaseStylishPointKind::kDamage] = 40.0f;
}
