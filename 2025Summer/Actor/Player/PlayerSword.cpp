#include "PlayerSword.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Player.h"
#include "Rigid.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "ConstantBufferIndex.h"
#include "ShaderDraw.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"
#include <cassert>
#include <cmath>
#include "Camera.h"

namespace
{
	constexpr float kSwordLength = 200.0f;
	constexpr float kSowrdRadius = 30.0f;

	const std::string kModelName = "Data/Model/greatsword.mv1";

	const std::string kRightWeaponFrame = "mixamorig:RightWeaponRig";

	constexpr float kDrag = 0.0f;

	const MATRIX kHandPosOffset = MGetTranslate({0, -20, 0});

	const std::string kTrajectoryEffectName = "SwordTrajectory.efkefc";
	const std::string kAppearEffectName = "SwordAppear.efkefc";
	constexpr int kStopFrame = 3;

	constexpr float kSwordAppearSpeed = 0.1f;
	constexpr float kSwordDisAppearSpeed = -0.02f;

	constexpr int kChargeGaugeBasePoint = 1;
	// 与ダメージのこの分だけ必殺技を増加
	constexpr float kSpecialAttackAttackPowerMult = 0.01f;

	constexpr int kShakeFrame = 5;
	constexpr int kShakeStrength = 5;

	const std::string kHitEffectName = "HitEffect.efkefc";
}

PlayerSword::PlayerSword() :
	m_isExisting(false),
	m_cBuffH(-1),
	m_cBuff(nullptr),
	m_frame(0),
	m_colTex(-1),
	m_dissolveTex(-1),
	m_psH(-1),
	m_vsH(-1)
{
}

PlayerSword::~PlayerSword()
{
	DeleteShaderConstantBuffer(m_cBuffH);
	DeleteShader(m_psH);
	DeleteShader(m_vsH);
	DeleteGraph(m_dissolveTex);

	if (m_effect.expired()) return;

	m_effect.lock()->Kill();
}

void PlayerSword::Init(std::weak_ptr<Player> player)
{
	auto cCol = std::make_shared<CapsuleCollider>();
	cCol->Init(m_pos, m_pos + Vector3::Up() * kSwordLength, 10, true, false, kSowrdRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);
	rigid->SetUseGravity(false);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(cCol, rigid);

	m_model = std::make_shared<Model>();
	m_model->Init(kModelName);

	m_player = player;

	m_kind = ActorKind::kPlayerAttack;

	m_psH = LoadPixelShader("Data/Shader/SwordDissolve.pso");
	assert(m_psH != -1);
	m_vsH = LoadVertexShader("Data/Shader/MV1VertexShader.vso");
	assert(m_vsH != -1);
	m_colTex = LoadGraph("Data/Image/dia.png");
	assert(m_colTex != -1);
	m_dissolveTex = LoadGraph("Data/Image/pattern.png");
	assert(m_dissolveTex != -1);

	m_cBuffH = CreateShaderConstantBuffer(sizeof(SwordCBuff));
	m_cBuff = (SwordCBuff*)GetBufferShaderConstantBuffer(m_cBuffH);
	SetCBuffStatus();
}

void PlayerSword::Update()
{
	auto before = m_dissolveParam;
	m_dissolveParam += m_isExisting ? kSwordAppearSpeed : kSwordDisAppearSpeed;

	// ディゾルブが0から変化した瞬間エフェクト
	if (before.IsMin() && !m_dissolveParam.IsMin())
	{
		EffectManager::GetInstance().GenerateEffect(kAppearEffectName, m_pos);
	}

	// バウンディングボックスを更新
	SetCBuffStatus();

	++m_frame;
}

void PlayerSword::Draw() const
{
	SetShaderConstantBuffer(m_cBuffH, DX_SHADERTYPE_PIXEL, static_cast<int>(CbuffIndex::kSwordDissolve));
	ShaderDraw::DrawModel(m_model, m_psH, m_vsH, m_colTex, m_dissolveTex);

#if _DEBUG
	//m_collidable->GetCol().Draw();
	auto& cCol = static_cast<CapsuleCollider&>(m_collidable->GetCol());

	auto color = m_collidable->GetCol().IsValid() ? 0xff0000 : 0x00ff00;

	DrawCapsule3D(cCol.StartPos(), cCol.EndPos(), kSowrdRadius, 10, color,0xffffff, false);
#endif
}

void PlayerSword::OnCollisionEnter(const std::shared_ptr<Actor> other)
{
	// 敵に当たったら
	if (other->GetKind() == ActorKind::kEnemy)
	{
		// プレイヤーにヒットストップをかける
		m_player.lock()->SetStopFrame(kStopFrame);
		// ちょっと画面振動
		m_player.lock()->m_camera.lock()->SetShake(kShakeFrame, kShakeStrength);
		// エフェクトを出す
		EffectManager::GetInstance().GenerateEffect(kHitEffectName, m_pos);

		// プレイヤーの必殺技ゲージを増加
		m_player.lock()->ChargeSpecialGauge(kChargeGaugeBasePoint + static_cast<int>(m_attackPower * kSpecialAttackAttackPowerMult));
	}
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

	auto capsuleEnd = m_pos + rigDir * kSwordLength;

	cCol.SetPos(m_pos, capsuleEnd);

	// エフェクトの位置を設定
	if (!m_effect.expired())
	{
		m_effect.lock()->SetPos(capsuleEnd);
	}
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
	m_collidable->GetCol().ValidCol();
	// ここでエフェクトを生成
	m_effect = EffectManager::GetInstance().GenerateEffect(kTrajectoryEffectName, {0,0,0});
}

void PlayerSword::ColDisable()
{
	m_collidable->GetCol().InvalidCol();

	if (m_effect.expired()) return;

	m_effect.reset();
}

void PlayerSword::SetCBuffStatus()
{
	m_cBuff->minPos = m_model->GetModelBBMin();
	m_cBuff->maxPos = m_model->GetModelBBMax();
	m_cBuff->time = m_dissolveParam.Value();
	UpdateShaderConstantBuffer(m_cBuffH);
}