#include "PlayerSword.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Player.h"
#include "Rigid.h"
#include <DxLib.h>
#include "AnimationModel.h"

namespace
{
	constexpr float kSwordLength = 200.0f;
	constexpr float kSowrdRadius = 30.0f;

	const std::string kModelName = "Data/Model/greatsword.mv1";

	const std::string kRightWeaponFrame = "mixamorig:RightWeaponRig";

	const PhysicalMaterial physiMat =
	{
		0,0,0
	};

	const MATRIX kHandPosOffset = MGetTranslate({0, -20, 0});
}

PlayerSword::PlayerSword() :
	Actor(false)
{
}

void PlayerSword::Init(std::weak_ptr<Player> player)
{
	auto cCol = std::make_shared<CapsuleCollider>();
	cCol->Init(m_pos, m_pos + Vector3::Up() * kSwordLength, 10, true, false, kSowrdRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(physiMat);
	rigid->SetUseGravity(false);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(cCol, rigid);

	m_model = std::make_shared<Model>();
	m_model->Init(kModelName);

	m_player = player;
}

void PlayerSword::Update()
{
	// 剣の位置、向き、回転をとりたい
	auto weaponMat = m_player.lock()->m_model->GetFrameMatrix(kRightWeaponFrame);
	
	// 先に位置補正
	weaponMat = MMult(kHandPosOffset, weaponMat);

	m_model->SetMatrix(weaponMat);
}

void PlayerSword::Draw() const
{
	m_model->Draw();

	m_collidable->GetCol().Draw();
}

void PlayerSword::OnCollision(const std::shared_ptr<Actor> other)
{
}

void PlayerSword::CommitMove()
{
	auto vel = m_collidable->UpdateRigid();
	m_pos += vel;
	
	m_collidable->SetPos(m_pos);
	//m_model->SetPos(m_pos);
}