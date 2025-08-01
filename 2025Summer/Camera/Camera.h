#pragma once
#include "../Utility/RangeLimitedValue.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector2.h"
#include <memory>
#include <list>

class Player;

namespace DxLib
{
	struct tagMATRIX;
}


class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw_Debug() const;

	void SetTargetPos(const Vector3& targetPos);
	void SetLerpPos(const Vector3& pos);
	// Y�����targetPos�𒆐S�ɉ�]
	void RotateCameraUpVecY(const float rad);
	// �J�������㉺�ɉ�]������
	void RotateCameraV(const float rad);
	// pos��targetPos�̒P�ʃx�N�g��
	Vector3 GetCameraDir() const;
	// XZ���ʂɌ������J�����̌������擾
	Vector3 GetCameraDirXZ() const;
	// �^����ꂽ�x�N�g�����J������XZ���ʂ̌����ɉ�]������
	// �������ɓ����ꂽ���W�����J�����̉��s�Ƃ��Đݒ肷��
	// ��Fvec2={0,0,1} �� z+�������J�����̉��s�ɂȂ�
	Vector3 RotateVecToCameraDirXZ(const Vector3& vec, const Vector3& vec2);
	void SetTargetFoV(const float deg);
	void SetTargetDistance(const float dist);
	float GetTargetDistance() const;
	float GetTargetDistanceXZ() const;
	Vector3 GetTargetPos() const { return m_targetPos; }
	float GetCameraNearFarLength() const;
	void SetCameraDistanceDefault();
	// �J�����̐U��
	void SetShake(const int frame, const int strength);

private:
	// �ǂ��Ɍ�����
	Vector3 m_targetPos;
	// ���ۂɌ���ʒu
	Vector3 m_lerpedTargetPos;
	// �����_���猩���J�����̌���
	Vector3 m_targetToCamera;
	// �����_�ƃJ�����̋���
	float m_targetDistance;
	float m_lerpedTargetDistance;
	// ����p�����炩�ɕω���������
	float m_FoV;
	float m_targetFoV;
	Vector2 m_cameraVel;

	// �J�����̐U����
	Vector3 m_shakePos;
	int m_shakeFrame;
	int m_startFrame;
	int m_shakeStrength;

	// �J�������ڎw�����W
	Vector3 m_targetCameraPos;
	Vector3 m_lerpedCameraPos;

private:
	void Rotate();
	void SetShakePos();
};