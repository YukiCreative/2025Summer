#pragma once
#include "../Geometry/Vector3.h"

//DxLib�̉e�@�\���g���₷��������
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Init(const Vector3& areaMin, const Vector3& areaMax);

	void StartShadowMapDraw(const Vector3& lightDir);

	// ���̊Ԃ�Draw������Ɖe�̂ق��ɑ�����

	void EndShadowMapDraw();

	// �����Œʏ�ʂ�̃��f���̕`��

	void UnsetShadowMap();

	// ���̌�͍Ă�Start����܂ŉe�Ȃ��̕`��

private:

	int m_handle;
};

