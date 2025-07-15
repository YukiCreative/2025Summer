#pragma once
#include <memory>

class EnemyPlant;

class EnemyPlantState : public std::enable_shared_from_this<EnemyPlantState>
{
public:
	EnemyPlantState(std::weak_ptr<EnemyPlant> parent) : m_parent(parent) {}
	virtual ~EnemyPlantState() {}

	virtual std::shared_ptr<EnemyPlantState> Update() abstract;

protected:

	std::weak_ptr<EnemyPlant> m_parent;

private:

	// ‹¤’Ê‚Åg‚¢‚½‚¢ŠÖ”‚Æ‚©
};