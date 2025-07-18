#pragma once
#include <memory>
#include <string>

class EnemyElite;

class EnemyEliteState : public std::enable_shared_from_this<EnemyEliteState>
{
public:
	EnemyEliteState(std::weak_ptr<EnemyElite> parent) :
		m_parent(parent),
		m_frame(0)
	{
	}

	virtual ~EnemyEliteState() {}

	virtual std::shared_ptr<EnemyEliteState> Update() abstract;

protected:

	std::weak_ptr<EnemyElite> m_parent;
	int m_frame;

private:

	// 共通で使いたい関数とか
};

