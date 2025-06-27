#pragma once
#include "VirtualCamera.h"
#include "Vector2.h"

// ��]����
class RotateCamera : public VirtualCamera
{
public:

	RotateCamera();

	void Init(const Vector3& target, const Vector3& targetToCamera, const float cameraDistance);

	void Update() override;

	void AddVel(const Vector2& vel);
	void SetVel(const Vector2& vel);
	void SetCameraDistance(const float dist);

	// ���܂Ȃ������̑���͍폜�����Ă�������
	void SetPos(const Vector3& pos) = delete;

private:

	Vector2 m_vel;

	// �����I��targetPos���炱�̃x�N�g���������L�΂����ʒu��pos�ɑ������
	// �J������]�͂������]������Ίy�Ɏ����ł���
	Vector3 m_targetToCamera;
	// �����Ƌ����𕪗�����΋����̕ύX���y�ɂȂ�
	float m_lerpedCameraDistance;
	float m_cameraDistance;

private:

	void Rotate();
	void EasingDistance();
};

