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
#include "../Enemy/Enemy.h"
#include <DxLib.h>

namespace
{
	constexpr int kLifeTime = 100;
	constexpr float kRadius = 1500.0f;
	const Vector3 kKnockbackPower = { 0.0f, 0.0f, 100.0f };
	constexpr float kAttackPower = 2000.0f;
	constexpr int kHitStopFrame = 100;
	const Vector2 kDrag = { 0.0f, 0.0f };
	const std::string kEffectName = "SpecialAttack.efkefc";
	constexpr float kCameraDistance = 800.0f;
	constexpr float kStunPower = 100.0f;

	// êUìÆ
	constexpr int kStartShakeFrame = 10;
	constexpr int kStartShakeStrength = 100;
	constexpr int kFinishShakeFrame = 30;
	constexpr int kFinishShakeStrength = 100;
}

PlayerSpecialAttackCol::PlayerSpecialAttackCol() :
	PlayerAttackCol(),
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
	m_stunPower = kStunPower;
	m_parent = parent;

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

	// ìGÇ™ñ≥ìGÇ»ÇÁÉXÉLÉbÉv
	auto enemy = std::static_pointer_cast<Enemy>(other);
	if (enemy->IsInvincible()) return;

	other->SetStopFrame(kHitStopFrame);

	// ÉmÉbÉNÉoÉbÉN
	// Ç±ÇÃçUåÇÇÃíÜêSÇ©ÇÁï˙éÀèÛÇ…îÚÇŒÇ∑
	Vector3 thisToE = enemy->GetPos() - m_pos;
	enemy->KnockBack(VTransformSR(m_knockbackPower, MGetRotVec2(Vector3::Foward(), thisToE)));

	// spëùâ¡
	StylishRank::GetInstance().IncreaseStylishPoint(IncreaseStylishPointKind::kSpecialAttack);

	enemy->OnDamage(weak_from_this());
}
