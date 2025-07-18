#include "PlayerShockWave.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Rigid.h"
#include <DxLib.h>

namespace
{
	constexpr float kMoveSpeed = 100.0f;

	constexpr float kColLength = 400.0f;
	constexpr float kColRadius = 50.0f;

	constexpr int kLifeTime = 40;
	constexpr int kResidueFrame = 1;

	constexpr float kDrag = 0.0f;
}

PlayerShockWave::PlayerShockWave() :
	AttackCol(),
	m_frame(0)
{
}

PlayerShockWave::~PlayerShockWave()
{
}

void PlayerShockWave::Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback)
{
	m_kind = ActorKind::kPlayerAttack;

	// コライダー初期化
	auto col = std::make_shared<CapsuleCollider>();
	col->Init({ 0,-kColLength * 0.5f, 0 }, {0,kColLength * 0.5f, 0}, 0, true, true, kColRadius);
	// 回転させてから平行移動
	col->SetPos(VTransformSR(col->StartPos(), rot), VTransformSR(col->EndPos(), rot));
	col->SetPos(initPos);
	m_pos = col->GetPos();
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_attackPower = atk;
	m_knockbackPower = knockback;

	// これでコピーがshared_ptrとして作られる
	m_dir = std::make_shared<MATRIX>(rot);
}

void PlayerShockWave::Update()
{
	// 一定速度で移動
	m_collidable->SetVel(VTransformSR({ 0,0,kMoveSpeed }, *m_dir));

	// 一定時間で消える
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
	// 敵に当たったら消滅
	if (other->GetKind() == ActorKind::kEnemy)
	{
		// ちょっとだけ猶予を持たせて、複数体の敵に当たるようにする

		m_frame = kLifeTime - kResidueFrame;
	}
}
