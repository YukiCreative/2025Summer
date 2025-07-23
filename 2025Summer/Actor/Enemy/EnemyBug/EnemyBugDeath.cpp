#include "EnemyBugDeath.h"
#include "EnemyBug.h"
#include "Geometry.h"

namespace
{
	// �����������[�V����������񂾂���g����
	const std::string kFrontDeath = "Armature|DownFront";
	const std::string kBackDeath  = "Armature|DownBack";
	const std::string kLeftDeath  = "Armature|DownLeft";
	const std::string kRightDeath = "Armature|DownRight";

	constexpr int kAnimFrameOffset = 60;
}

EnemyBugDeath::EnemyBugDeath(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_animTotalFrame(0),
	m_frame(0)
{
	// �A�j���[�V����
	// �v���C���[�Ƃ̌����ɂ���ē|��������ς��悤
	auto eToPN = m_parent.lock()->EnemyToPlayer().GetNormalize().XZ();
	auto dir = -m_parent.lock()->GetDir().XZ(); // �Ȃ����t�ɂȂ�̂Ō������t�ɂ���
	auto eToPRad = Geometry::Corner(eToPN, dir);
	auto cross = dir.Cross(eToPN);

	// �O
	if (eToPRad <= DX_PI_F * 0.25f)
	{
		m_parent.lock()->ChangeAnim(kFrontDeath, false);
	}
	else if (eToPRad >= DX_PI_F * 0.75f) // ���
	{
		m_parent.lock()->ChangeAnim(kBackDeath, false);
	}
	else if (cross.y > 0)// �E
	{
		m_parent.lock()->ChangeAnim(kRightDeath, false);
	}
	else
	{
		m_parent.lock()->ChangeAnim(kLeftDeath, false);
	}


	m_animTotalFrame = m_parent.lock()->GetAinmTotalTime();
	// ���G��
	m_parent.lock()->SetInvincibility(true);
	m_parent.lock()->StartDissolve();
}

EnemyBugDeath::~EnemyBugDeath()
{
}

std::shared_ptr<EnemyBugState> EnemyBugDeath::Update()
{
	// �A�j���[�V�������I����Ă��΂炭������
	if (m_frame > m_animTotalFrame + kAnimFrameOffset)
	{
		m_parent.lock()->OnDeath();
	}

	++m_frame;

	return shared_from_this();
}
