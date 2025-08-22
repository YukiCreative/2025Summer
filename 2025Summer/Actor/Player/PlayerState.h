#pragma once
#include <memory>
#include "../../Geometry/Vector3.h"

class Player;

// PlayerStateで共有したい定数はここに入れよう
namespace
{
	constexpr float kMoveThreshold = 0.001f;
	const Vector3 kCameraTargetOffset = { 0,150,0 };
	constexpr float kLockOnWalkSpeed = 0.002f;
	const Vector3 kJumpForce = { 0.0f, 70.0f, 0.0f };
	constexpr float kStartDragY = 0.1f;
	constexpr int kSetDragDefaultFrame = 10;
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

	bool CanCrossState() const { return m_canCrossState; }

protected:

	std::weak_ptr<Player> m_player;

	bool m_canCrossState;

protected:
};

using FunctionEnter = std::function<void()>;
using FunctionUpdate = std::function<void(float)>;
using FunctionExit = std::function<void()>;

struct StateInfo
{
	FunctionEnter Enter;
	FunctionUpdate Update;
	FunctionExit Exit;
};

class StateMachine
{
public:
	void AddState(string, FunctionEnter, FunctionUpdate, FunctionExit);
	void Update(float DeltaTime)
	{
		//
	}
protected:
	map<string/*StateName*/, StateInfo> StateList;
};

class PlayerMove
{
protected:
	StateMachine StateMachine;

private:
};