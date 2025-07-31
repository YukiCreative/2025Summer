#pragma once
#include "UIBase.h"
#include "StylishEnum.h"

class Image;
class ArenaMode;

// �E�F�[�u�Ԃ̃��U���g���o���܂�
class WaveResult : public UIBase
{
public:
	WaveResult();
	~WaveResult();

	void Init();

	void Update() override;

	void Draw() const override;

	// ���̏�Ԃ��烊�U���g�o��
	void StartResult();

private:

	std::shared_ptr<Image> m_image;
	int m_fontH;

	// ��_��
	float m_damageAmount;
	// ����
	float m_second;
	// ���������N
	StylishRankKind m_rank;

	using Sequence_t = void (WaveResult::*)();
	Sequence_t m_sequence;

private:

	void Appear();
	void Wait();
	void Disappear();
};

