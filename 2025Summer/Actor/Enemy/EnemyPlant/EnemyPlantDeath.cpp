#include "EnemyPlantDeath.h"
#include "EnemyPlant.h"
#include "Geometry.h"

namespace
{
	const std::string kFrontDeath = "Armature|DownFront";
	const std::string kBackDeath  = "Armature|DownBack";
	const std::string kRightDeath = "Armature|DownRight";
	const std::string kLeftDeath  = "Armature|DownLeft";
}

EnemyPlantDeath::EnemyPlantDeath(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
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
	else if (cross.y < 0)// �E
	{
		m_parent.lock()->ChangeAnim(kRightDeath, false);
	}
	else
	{
		m_parent.lock()->ChangeAnim(kLeftDeath, false);
	}

	// ���G��
	m_parent.lock()->SetInvincibility(true);
}

EnemyPlantDeath::~EnemyPlantDeath()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDeath::Update()
{
	// �A�j���[�V�������I���܂őҋ@
	if (m_parent.lock()->IsEndAnim())
	{
		m_parent.lock()->OnDeath();
	}

	return shared_from_this();
}
