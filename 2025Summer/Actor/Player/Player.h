#pragma once
#include "Actor.h"

class AnimationModel;
class Camera;
class PlayerState;


// �A�N�Z�X���x����public�ɂȂ��Ă���̂́A�X�e�[�g�N���X�ɏ��𖾂��n������
// ���p���ցI
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	// �O�����猩�Ă������֐�

	Player();

	void Init(const std::weak_ptr<Camera> camera);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

public:
	// ������PlayerState�����Ō����������o�ϐ�
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerState> m_state;

public:
	// ������PlayerState�����Ō��Ăق����֐�
	void Move(const float moveSpeed);
	void CameraMove();
};

