#pragma once
#include "Actor.h"
#include "PlayerState.h"

class AnimationModel;
class Camera;

class Player : public Actor
{
public:
	Player();

	void Init(const std::weak_ptr<Camera> camera);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

private:

	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;

private:
	void Move();
};

