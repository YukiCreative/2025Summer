#pragma once
#include "Actor.h"

class AnimationModel;

class EnemyTest : public Actor
{
public:
	EnemyTest();

	void Init();

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollision(std::shared_ptr<Actor> other) override;

private:

	std::shared_ptr<AnimationModel> m_model;

private:
};

