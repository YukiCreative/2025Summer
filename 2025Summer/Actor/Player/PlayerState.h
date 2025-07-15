#pragma once
#include <memory>
#include "Vector3.h"

class Player;

// PlayerStateで共有したい定数はここに入れよう
namespace
{
	constexpr float kMoveThreshold = 0.001f;
	const Vector3 kCameraTargetOffset = { 0,150,0 };
	constexpr float kLockOnWalkSpeed = 0.002f;
}

// プレイヤーの状態をクラスに分ける
class PlayerState abstract : public std::enable_shared_from_this<PlayerState>
{
public:
	// 遷移時の処理をその状態側に書けるのが一番の利点かな
	PlayerState(std::weak_ptr<Player> parent);
	virtual ~PlayerState();

	// 遷移したとき、遷移した状態を返す
	// そうでないときは自分を返す
	virtual std::shared_ptr<PlayerState> Update() abstract;

protected:
	std::weak_ptr<Player> m_player;

protected:
};

