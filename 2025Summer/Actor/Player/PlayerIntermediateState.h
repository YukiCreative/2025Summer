#pragma once
#include "PlayerState.h"

// プレイヤーと状態の間に一枚入れているクラス
// 今のところロックオンしてるかそうでないかに使ってる
// より深い階層構造が必要になった時は設計を変更する必要がある
class PlayerIntermediateState : public std::enable_shared_from_this<PlayerIntermediateState>
{
public:
	PlayerIntermediateState(std::weak_ptr<Player> parent);
	~PlayerIntermediateState();

	// これが欲しかった
	// ダメージ状態を割り込みで実行する
	void SetStateDamage();
	// 死亡
	void SetStateDeath();

	virtual std::shared_ptr<PlayerIntermediateState> Update() abstract;

protected:

	// Update内で必ず実行するように
	void UpdateChildState();
	 
protected:

	std::shared_ptr<PlayerState> m_childState;
	std::weak_ptr<Player> m_player;

private:


};

