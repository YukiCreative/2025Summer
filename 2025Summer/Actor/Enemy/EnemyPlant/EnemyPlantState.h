#pragma once
#include <memory>
#include <string>

class EnemyPlant;

class EnemyPlantState : public std::enable_shared_from_this<EnemyPlantState>
{
public:
	EnemyPlantState(std::weak_ptr<EnemyPlant> parent) :
		m_parent(parent),
		m_frame(0)
	{

	}

	virtual ~EnemyPlantState() {}

	virtual std::shared_ptr<EnemyPlantState> Update() abstract;

protected:

	std::weak_ptr<EnemyPlant> m_parent;
	int m_frame;

private:

	// ‹¤’Ê‚Åg‚¢‚½‚¢ŠÖ”‚Æ‚©
};