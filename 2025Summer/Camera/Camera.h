#pragma once
#include "RangeLimitedValue.h"
#include "Vector3.h"
#include <memory>

class Player;

namespace DxLib
{
	struct tagMATRIX;
}

// DxLib�ɃJ��������������݂��Ȃ��̂ŃV���O���g���ł��ǂ����H
// �����ɍςނȂ炻�ꂪ��ԂȂ̂���
class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw_Debug() const;

	void SetTargetPos(const Vector3& targetPos);
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

private:
	// �ǂ��Ɍ�����
	Vector3 m_targetPos;
	// ���ۂɌ���ʒu
	Vector3 m_lerpedTargetPos;
	// �����_���猩���J�����̌���
	Vector3 m_targetToCamera;
	// �����_�ƃJ�����̋���
	float m_targetDistance;
	// ����p�����炩�ɕω���������
	float m_FoV;
	float m_targetFoV;
};