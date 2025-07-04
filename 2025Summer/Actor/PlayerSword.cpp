#include "PlayerSword.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Player.h"
#include "Rigid.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "ConstantBufferIndex.h"
#include "ShaderDraw.h"

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
	Actor(false),
	m_isExisting(false),
	m_attackPower(0),
	m_isAttacking(false),
	m_cBuffH(-1)
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

	m_kind = ActorKind::kPlayerAttack;

	m_cBuffH = CreateShaderConstantBuffer(sizeof(SwordCBuff));
	m_cBuff = (SwordCBuff*)GetBufferShaderConstantBuffer(m_cBuffH);
	m_cBuff->time = 0.0f;
	UpdateShaderConstantBuffer(m_cBuffH);
	SetShaderConstantBuffer(m_cBuffH, DX_SHADERTYPE_PIXEL, static_cast<int>(CbuffIndex::kSwordDissolve));
}

void PlayerSword::Update()
{
}

void PlayerSword::Draw() const
{
	if (m_isExisting)
	{
		m_model->Draw();
		//ShaderDraw::DrawModel(m_model);
	}

#if _DEBUG
	//m_collidable->GetCol().Draw();
	auto& cCol = static_cast<CapsuleCollider&>(m_collidable->GetCol());

	auto color = m_isAttacking ? 0xff0000 : 0x00ff00;

	DrawCapsule3D(cCol.StartPos(), cCol.EndPos(), kSowrdRadius, 10, color,0xffffff, false);
#endif
}

void PlayerSword::OnCollision(const std::shared_ptr<Actor> other)
{
}

void PlayerSword::CommitMove()
{
	// 剣の位置、向き、回転をとりたい
	auto weaponMat = m_player.lock()->m_model->GetFrameMatrix(kRightWeaponFrame);

	// 先に位置補正
	weaponMat = MMult(kHandPosOffset, weaponMat);

	m_model->SetMatrix(weaponMat);

	m_pos = MGetTranslateElem(weaponMat);

	const Vector3 rigDir = Vector3{ weaponMat.m[1][0],weaponMat.m[1][1],weaponMat.m[1][2] }.GetNormalize();

	auto& cCol = static_cast<CapsuleCollider&>(m_collidable->GetCol());

	cCol.SetPos(m_pos, m_pos + rigDir * kSwordLength);
}

void PlayerSword::Enable()
{
	m_isExisting = true;
}

void PlayerSword::Disable()
{
	m_isExisting = false;
}

void PlayerSword::ColEnable()
{
	m_isAttacking = true;
}

void PlayerSword::ColDisable()
{
	m_isAttacking = false;
}

void PlayerSword::AppearUpdate()
{
}

void PlayerSword::DisappearUpdate()
{
}
