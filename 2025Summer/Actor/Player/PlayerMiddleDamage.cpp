#include "../../Model/AnimationModel.h"
#include "../../General/Input.h"
#include "Player.h"
#include "NormalState/PlayerIdle.h"
#include "PlayerMiddleDamage.h"
#include "NormalState/PlayerMove.h"
#include "AttackState/PlayerSlashDown.h"
#include <DxLib.h>

namespace
{
	const std::string kStunAnimName = "Armature|MiddleDamage";

	// 移動可能になるフレーム
	constexpr int kEnableMoveFrame = 30;
	constexpr float kMoveSpeed = 0.001f;
}

PlayerMiddleDamage::PlayerMiddleDamage(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	auto p = m_player.lock();

	p->m_model->ChangeAnimation(kStunAnimName, false);
	// 剣を消す
	p->DisableSword();
	// 当たり判定も消す
	p->DisableSwordCol();
	m_canCrossState = true;
}

PlayerMiddleDamage::~PlayerMiddleDamage()
{
}

std::shared_ptr<PlayerState> PlayerMiddleDamage::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// アニメーションを完遂or途中で入力があったら遷移
	if (p->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	if (m_frame >= kEnableMoveFrame)
	{
		if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			return std::make_shared<PlayerMove>(m_player);
		}
		if (input.IsTrigger("Attack"))
		{
			return std::make_shared<PlayerSlashDown>(m_player);
		}

	}

	// ちょっとだけ動ける
	p->Move(kMoveSpeed);

	++m_frame;

	return shared_from_this();
}
