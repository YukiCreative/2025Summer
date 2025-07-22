#include "PlayerSpecialAttackCol.h"
#include "Collidable.h"
#include "SphereCollider.h"
#include "Rigid.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"
#include <string>
#include "Camera.h"
#include "Player.h"

namespace
{
	constexpr int kLifeTime = 10;
	constexpr float kRadius = 1500.0f;
	constexpr float kKnockbackPower = 100.0f;
	constexpr float kAttackPower = 2000.0f;
	constexpr int kHitStopFrame = 100;
	constexpr float kDrag = 0.0f;
	const std::string kEffectName = "SpecialAttack.efkefc";
	constexpr float kCameraDistance = 800.0f;
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

	// ‚±‚ê‚à‚ß‚ñ‚Ç‚­‚³‚­‚È‚Á‚Ä‚«‚½‚È
	auto col = std::make_shared<SphereCollider>();
	col->Init(initPos, 0, true, true, kRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_pos = initPos;

	EffectManager::GetInstance().GenerateEffect(kEffectName, m_pos);

	parent.lock()->m_camera.lock()->SetTargetDistance(kCameraDistance);
}

void PlayerSpecialAttackCol::Update()
{
	if (m_frame > kLifeTime)
	{
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
	// “GˆÈŠO‚É‚Í“–‚½‚ç‚È‚¢
	if (other->GetKind() != ActorKind::kEnemy) return;

	other->SetStopFrame(kHitStopFrame);
}
