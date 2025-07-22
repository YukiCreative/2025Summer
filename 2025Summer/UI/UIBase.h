#pragma once
#include <memory>
#include "Vector3.h"
#include "Vector2.h"

class Image;

// UI�̊��N���X
// 3D�̕`���ɕ`�悳���
class UIBase abstract
{
public:
	UIBase();

	virtual void Update() abstract;
	virtual void Draw() const abstract;

protected:

	// Z�ɂ���ďd�Ȃ鏇�Ԃ𑀍�ł��Ă���������
	// 7/16���݂͂���ȋ@�\�͂Ȃ�
	Vector3 m_pos;
};

