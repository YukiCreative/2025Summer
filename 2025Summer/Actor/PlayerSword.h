#pragma once
#include "Actor.h"
#include "Model.h"

class Player;

// ���f���Ɠ����蔻��̃Z�b�g
class PlayerSword : public Actor
{
public:
	PlayerSword();

	void Init(std::weak_ptr<Player> player);

	void Update() override;
	void Draw() const override;

	void OnCollision(const std::shared_ptr<Actor> other) override;

	void CommitMove() override;

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;
};

