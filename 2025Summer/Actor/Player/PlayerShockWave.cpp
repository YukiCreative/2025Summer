#include "PlayerShockWave.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Rigid.h"
#include "Player.h"
#include "EffekseerEffect.h"
#include "EffectManager.h"
#include "Geometry.h"
#include <DxLib.h>
#include "StylishRank.h"

namespace
{
	constexpr float kMoveSpeed = 100.0f;

	constexpr float kColLength = 400.0f;
	constexpr float kColRadius = 50.0f;

	constexpr int kLifeTime = 40;
	constexpr int kResidueFrame = 1;

	constexpr float kDrag = 0.0f;

	constexpr int kChargeGaugeBasePoint = 1;
	// —^ƒ_ƒ[ƒW‚Ì‚±‚Ì•ª‚¾‚¯•KE‹Z‚ğ‘‰Á
	constexpr float kSpecialAttackAttackPowerMult = 0.01f;

	const std::string kShockWaveEffect = "ShockWave.efkefc";
	const std::string kDisappearEffect = "ShockWaveDisappear.efkefc";
}

PlayerShockWave::PlayerShockWave() :
	AttackCol(),
	m_frame(0),
	m_rotateY(0.0f)
{
}

PlayerShockWave::~PlayerShockWave()
{
	if (m_effect.expired()) return;

	m_effect.lock()->Kill();
	auto effect = EffectManager::GetInstance().GenerateEffect(kDisappearEffect, m_pos);
	effect.lock()->SetRotate({0, m_rotateY, 0});
}

void PlayerShockWave::Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback,
	std::weak_ptr<Player> player)
{
	m_kind = ActorKind::kPlayerAttack;

	m_player = player;

	// ƒRƒ‰ƒCƒ_[‰Šú‰»
	auto col = std::make_shared<CapsuleCollider>();
	col->Init({ 0,-kColLength * 0.5f, 0 }, {0,kColLength * 0.5f, 0}, 0, true, true, kColRadius);
	// ‰ñ“]‚³‚¹‚Ä‚©‚ç•½sˆÚ“®
	col->SetPos(VTransformSR(col->StartPos(), rot), VTransformSR(col->EndPos(), rot));
	col->SetPos(initPos);
	m_pos = col->GetPos();
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_attackPower = atk;
	m_knockbackPower = knockback;

	// ‚±‚ê‚ÅƒRƒs[‚ªshared_ptr‚Æ‚µ‚Äì‚ç‚ê‚é
	m_dir = std::make_shared<MATRIX>(rot);

	m_effect = EffectManager::GetInstance().GenerateEffect(kShockWaveEffect, m_pos);
	// ‰ñ“]
	m_rotateY = Geometry::Corner(Vector3::Foward(), m_player.lock()->GetDirection());
	if (m_player.lock()->GetDirection().Cross(Vector3::Foward()).y > 0) m_rotateY *= -1;
	m_effect.lock()->SetRotate({ 0, m_rotateY, 0 });
}

void PlayerShockWave::Update()
{
	// ˆê’è‘¬“x‚ÅˆÚ“®
	m_collidable->SetVel(VTransformSR({ 0,0,kMoveSpeed }, *m_dir));

	m_effect.lock()->SetPos(m_pos);

	// ˆê’èŠÔ‚ÅÁ‚¦‚é
	if (m_frame >= kLifeTime)
	{
		Destroy();
	}
	++m_frame;
}

void PlayerShockWave::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void PlayerShockWave::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	m_collidable->SetPos(m_pos);
}

void PlayerShockWave::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	// “G‚É“–‚½‚Á‚½‚çÁ–Å
	if (other->GetKind() == ActorKind::kEnemy)
	{
		// ‚¿‚å‚Á‚Æ‚¾‚¯—P—\‚ğ‚½‚¹‚ÄA•¡”‘Ì‚Ì“G‚É“–‚½‚é‚æ‚¤‚É‚·‚é

		m_frame = kLifeTime - kResidueFrame;

		// ƒQ[ƒW‘‰Á
		m_player.lock()->ChargeSpecialGauge(kChargeGaugeBasePoint + static_cast<int>(m_attackPower * kSpecialAttackAttackPowerMult));

		// sp‘‰Á
		StylishRank::GetInstance().IncreaseStylishPoint(IncreaseStylishPointKind::kShockWave);
	}
}
