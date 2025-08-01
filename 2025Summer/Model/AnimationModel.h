#pragma once
#include <memory>
#include <string>
#include "../Geometry/Vector3.h"
#include "../Utility/RangeLimitedValue.h"

class Model;

namespace DxLib
{
	struct tagMATRIX;
}

// 3D���f�������������Ɏg���₷���A�j���[�V����������
// Model���p�����Ă��܂����l����
class AnimationModel
{
public:
	AnimationModel();
	~AnimationModel();

	void Init(const std::string& path, const float animSpeed);
	// ���łɂ���n���h�����Q�Ƃ���p�^�[��
	// �f�X�g���N�^�Ń��f�����폜���Ȃ�
	void Init(const int handle, const float animSpeed);

	void Update();
	void Draw(const Vector3& drawPos) const;
	void Draw() const;

	void Stop() { m_isPlaying = false; }
	void Play() { m_isPlaying = true; }
	const bool IsPlaying() const { return m_isPlaying; }
	// �A�j���[�V�������I����Ă��邩�ǂ���
	const bool IsEnd() const { return !m_isPlaying; }
	// �A�j���[�V�����̃u�����h���ł��܂��I
	void ChangeAnimation(const int animId, bool isLoop = true);
	void ChangeAnimation(const std::string& animName, bool isLoop = true);
	// �����炭ModelViewer�̎��ԒP��
	int GetAnimTotalTime() const;

	// ���̕�Model�N���X�̒��p
	// Y������ɉ�]
	void RotateUpVecY(const float rad);
	void RotateUpVecZ(const float rad);
	void SetPos(const Vector3& pos);
	DxLib::tagMATRIX GetMatrix() const;
	void SetMatrix(const DxLib::tagMATRIX& mat);
	// ���̃A�j���[�V�������m���߂�Ƃ��Ɏg��
	const bool CheckAnimName(const std::string name);
	// ���f���̍s���Z���x�N�g�����������̂�Ԃ�
	// �v���C���[�̃��f�������]���Ă����̂ŁA����ɍ��킹�ĕ����𔽓]���Ă��܂��Ă���̂Œ���(���X�S���ύX�ł��Ȃ��c)
	const Vector3 GetDirection();
	// 0�`1
	void SetAnimBlendSpeed(const float speed) { m_animBlendSpeed = speed; }
	Vector3 GetFramePosition(const std::string& frameName) const;
	Vector3 GetFramePosition(const int frameIndex) const;
	DxLib::tagMATRIX GetFrameMatrix(const std::string& frameName) const;
	void SetScale(const Vector3& scale);
	const int GetFrameNum() const;

private:
	std::shared_ptr<Model> m_model;

	int m_beforeAnimHandle;
	int m_nowAnimHandle;

	using BlendRate_t = RangeLimitedValue<float, 0.0f, 1.0f>;
	BlendRate_t m_blendRate;

	float m_animBlendSpeed;
	float m_playTime;
	float m_playSpeed;
	bool m_isPlaying;
	float m_animTotalTime;
	bool m_isLoop;
};

