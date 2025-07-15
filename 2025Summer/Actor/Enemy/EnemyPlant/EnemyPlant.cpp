#include "EnemyPlant.h"
#include "AnimationModel.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Rigid.h"

namespace
{
	constexpr float kInitHP = 750.0f;
	const std::string kModelName = "Plant.mv1";

	constexpr float kAnimSpeed = 30.0f;

	// コライダー
	const Vector3 kCapsuleEndOffset = {0.0f, 150.0f, 0.0f};
	constexpr float kWeight = 10.0f;
	constexpr float kRadius = 40.0f;

	const PhysicalMaterial kMat =
	{
		0.1f, 0.05f, 0.1f
	};
}

EnemyPlant::EnemyPlant()
{
}

void EnemyPlant::Init(const std::weak_ptr<Player> player, const Vector3& initPos)
{
	Enemy::Init(player, initPos, kInitHP);

	// モデル
	m_model = std::make_shared<AnimationModel>();
	m_model->Init(kModelName, kAnimSpeed);

	// 当たり判定
	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndOffset, kWeight, false, false, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// 初期状態
	//m_state = 
}

void EnemyPlant::Update()
{
}

void EnemyPlant::Draw() const
{
}

void EnemyPlant::CommitMove()
{
}