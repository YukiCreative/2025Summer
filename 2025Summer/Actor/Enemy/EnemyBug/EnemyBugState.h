#pragma once
#include <memory>

class EnemyBug;

// 各敵もプレイヤーと同じように状態クラスで管理する
class EnemyBugState abstract : public std::enable_shared_from_this<EnemyBugState>
{
public:
	EnemyBugState(std::weak_ptr<EnemyBug> parent) : m_parent(parent) {}
	virtual ~EnemyBugState() {}

	virtual std::shared_ptr<EnemyBugState> Update() abstract;

protected:

	std::weak_ptr<EnemyBug> m_parent;

private:

	// 共通で使いたい関数とか
};

