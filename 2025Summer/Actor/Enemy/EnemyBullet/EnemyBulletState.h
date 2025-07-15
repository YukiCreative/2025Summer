#pragma once
#include <memory>

class EnemyBullet;

class EnemyBulletState : public std::enable_shared_from_this<EnemyBulletState>
{
public:
	EnemyBulletState(std::weak_ptr<EnemyBullet> parent) : m_parent(parent) {}
	virtual ~EnemyBulletState() {}

	virtual std::shared_ptr<EnemyBulletState> Update() abstract;

protected:

	std::weak_ptr<EnemyBullet> m_parent;

private:

	// ‹¤’Ê‚Åg‚¢‚½‚¢ŠÖ”‚Æ‚©
};