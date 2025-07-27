#include "StylishRank.h"
#include <cassert>

namespace
{
	constexpr size_t kMaxHistory = 10;
}

StylishRank::StylishRank()
{
}

void StylishRank::Init()
{
	InitMap();
}

void StylishRank::Update()
{
	// ��肽������
	// ���ԂŃX�^�C���b�V���|�C���g��������������
	// �������Ă��Ȃ����Ԃ������قǌ����ʂ𑝂₷
	// ���������玞�ԃ��Z�b�g
	// �������x�ɂ͍ő�l������
}

void StylishRank::IncreaseStylishPoint(const IncreaseStylishPointKind kind)
{
	assert(m_increasePointMap.contains(kind) && "�Ή����Ă�񋓑̗̂v�f��Map�ɂȂ�������");

	// ��肽������
	// �A�����ē������@�ŉ��_����Ă�����_�ʂ����炷
	// ���̂��߂ɂǂ�ȉ��_���@���L������
	// �������ʂ��ǂꂭ�炢���������邩�͂܂��l����(���A���^�C���ł����̂��A�s�����Ȃ��Ƃ����Ȃ��̂�)
	// �܂��A�ߋ��������ɑ��݂��Ȃ��������_���@�̓{�[�i�X�Ƃ������Ă�����

	m_sp += m_increasePointMap[kind];

	if (m_stylishActionHistory.size() > kMaxHistory)
	{
		m_stylishActionHistory.pop_front();
	}
	m_stylishActionHistory.emplace_back(kind);
}

void StylishRank::DecreaseStylishPoint(const DecreaseStylishPointKind kind)
{
	assert(m_decreasePointMap.contains(kind) && "�Ή����Ă�񋓑̗̂v�f��Map�ɂȂ�������");
	m_sp -= m_decreasePointMap[kind];
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
