#include "PlayerSpecialAttackCol.h"
#include "../../Physics/Collidable.h"
#include "../../Physics/Collider/SphereCollider.h"
#include "../../Physics/Rigid.h"
#include "../../Effect/EffekseerEffect.h"
#include "../../Effect/EffectManager.h"
#include <string>
#include "../../Camera/Camera.h"
#include "Player.h"
#include "../../GameManagement/Score/StylishRank.h"

namespace
{
	constexpr int kLifeTime = 100;
	constexpr float kRadius = 1500.0f;
	constexpr float kKnockbackPower = 100.0f;
	constexpr float kAttackPower = 2000.0f;
	constexpr int kHitStopFrame = 100;
	constexpr float kDrag = 0.0f;
	const std::string kEffectName = "SpecialAttack.efkefc";
	constexpr float kCameraDistance = 800.0f;

	// êUìÆ
	constexpr int kStartShakeFrame = 10;
	constexpr int kStartShakeStrength = 100;
	constexpr int kFinishShakeFrame = 30;
	constexpr int kFinishShakeStrength = 100;
}

PlayerSpecialAttackCol::PlayerSpecialAttackCol() :
	AttackCol(),
	m_frame(0)
{
}

PlayerSpecialAttackCol::~PlayerSpecialAttackCol()
{
}

void PlayerSpecialAttackCol::Init(std::weak_ptr<Player> parent, const Vector3& initPos)
{
	m_kind = ActorKind::kPlayerAttack;

	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;
	m_parent = parent;

	// Ç±ÇÍÇ‡ÇﬂÇÒÇ«Ç≠Ç≥Ç≠Ç»Ç¡ÇƒÇ´ÇΩÇ»
	auto col = std::make_shared<SphereCollider>();
	col->Init(initPos, 0, true, true, kRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_pos = initPos;

	EffectManager::GetInstance().GenerateEffect(kEffectName, m_pos);

	m_parent.lock()->m_camera.lock()->SetTargetDistance(kCameraDistance);

	m_parent.lock()->m_camera.lock()->SetShake(kStartShakeFrame, kStartShakeStrength);
}

void PlayerSpecialAttackCol::Update()
{
	if (m_frame > kLifeTime)
	{
		m_parent.lock()->m_camera.lock()->SetShake(kFinishShakeFrame, kFinishShakeStrength);
		Destroy();
	}

	++m_frame;
}

void PlayerSpecialAttackCol::Draw() const
{
	m_collidable->GetCol().Draw();
}

void PlayerSpecialAttackCol::CommitMove()
{
}

void PlayerSpecialAttackCol::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	// ìGà»äOÇ…ÇÕìñÇΩÇÁÇ»Ç¢
	if (other->GetKind() != ActorKind::kEnemy) return;

	other->SetStopFrame(kHitStopFrame);

	// spëùâ¡
	StylishRank::GetInstance().IncreaseStylishPoint(IncreaseStylishPointKind::kSpecialAttack);
}
