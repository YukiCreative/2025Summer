#include "AnimationModel.h"
#include <DxLib.h>
#include "Model.h"
#include <cassert>
#include "../General/Game.h"

void AnimationModel::ChangeAnimation(const int animId, bool isLoop)
{
	m_isLoop = isLoop;
	m_isPlaying = true;

	// �܂���O�̃A�j���[�V�������f�^�b�`
	MV1DetachAnim(m_model->m_handle, m_beforeAnimHandle);

	// �ЂƂO�̃A�j���[�V���������Ƃ�
	m_beforeAnimHandle = m_nowAnimHandle;

	// �V�����̂��A�^�b�`
	m_nowAnimHandle = MV1AttachAnim(m_model->m_handle, animId);
	assert(m_nowAnimHandle != -1 && "�A�j���[�V�����̃A�^�b�`�Ɏ��s����");

	m_playTime = 0;

	// �A�j���[�V�����̎��Ԃ�c�����Ă���
	m_animTotalTime = MV1GetAttachAnimTotalTime(m_model->m_handle, m_nowAnimHandle);

	// �A�j���[�V�����̃u�����h����ݒ�
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_beforeAnimHandle, 1.0f);
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_nowAnimHandle, 0.0f);

	// �u�����h�������Z�b�g
	m_blendRate = 0.0f;
}

void AnimationModel::ChangeAnimation(const std::string& animName, bool isLoop)
{
	const int animId = MV1GetAnimIndex(m_model->m_handle, animName.c_str());
	ChangeAnimation(animId, isLoop);
}

void AnimationModel::RotateUpVecY(const float rad)
{
	m_model->RotateUpVecY(rad);
}

void AnimationModel::RotateUpVecZ(const float rad)
{
	m_model->RotateUpVecZ(rad);
}

void AnimationModel::SetPos(const Vector3& pos)
{
	m_model->SetPos(pos);
}

DxLib::tagMATRIX AnimationModel::GetMatrix() const
{
	return m_model->GetMatrix();
}

void AnimationModel::SetMatrix(const DxLib::tagMATRIX& mat)
{
	m_model->SetMatrix(mat);
}

const bool AnimationModel::CheckAnimName(const std::string name)
{
	int a = MV1GetAnimIndex(m_model->m_handle, name.c_str());
	int b = MV1GetAttachAnim(m_model->m_handle, m_nowAnimHandle);
	return  a == b;
}

const Vector3 AnimationModel::GetDirection()
{
	auto mat = m_model->GetMatrix();
	return
	{
		-mat.m[2][0],
		-mat.m[2][1],
		-mat.m[2][2],
	};
}

Vector3 AnimationModel::GetFramePosition(const std::string& frameName) const
{
	return m_model->GetFramePosition(frameName);
}

Vector3 AnimationModel::GetFramePosition(const int frameIndex) const
{
	return m_model->GetFramePosition(frameIndex);
}

DxLib::tagMATRIX AnimationModel::GetFrameMatrix(const std::string& frameName) const
{
	return m_model->GetFrameMatrix(frameName);
}

void AnimationModel::SetScale(const Vector3& scale)
{
	m_model->SetScale(scale);
}

const int AnimationModel::GetFrameNum() const
{
	return m_model->GetFrameNum();
}

int AnimationModel::GetAnimTotalTime() const
{
	return static_cast<int>(MV1GetAttachAnimTotalTime(m_model->m_handle, m_nowAnimHandle));
}

AnimationModel::AnimationModel() :
	m_beforeAnimHandle(-1),
	m_nowAnimHandle(-1),
	m_isPlaying(false),
	m_playTime(0),
	m_animTotalTime(0),
	m_isLoop(true),
	m_playSpeed(1.0f),
	m_blendRate(0),
	m_animBlendSpeed(0.1f)
{
}

AnimationModel::~AnimationModel()
{
}

void AnimationModel::Init(const std::string& path, const float animSpeed)
{
	m_model = std::make_shared<Model>();
	m_model->Init(path);
	m_playSpeed = animSpeed;
}

void AnimationModel::Init(const int handle, const float animSpeed)
{
	m_model = std::make_shared<Model>();
	m_model->Init(handle);
	m_playSpeed = animSpeed;
}

void AnimationModel::Update()
{
	if (!m_isPlaying) return;

	// �A�j���[�V�����̎��Ԃ�i�߂�
	// �����P�ʂ̓~���b
	m_playTime += m_playSpeed / Game::kFrameRate;

	if (m_playTime > m_animTotalTime)
	{
		// ���[�v���Ă����Ȃ烋�[�v
		if (m_isLoop)
		{
			m_playTime -= m_animTotalTime;
		}
		else
		{
			m_isPlaying = false;
		}
	}

	// �u�����h����ݒ�
	m_blendRate += m_animBlendSpeed;

	// �A�j���[�V�����̃u�����h����ݒ�
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_beforeAnimHandle, 1 - m_blendRate.Value());
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_nowAnimHandle, m_blendRate.Value());

	// ���̃A�j���[�V�����͐i�߂Ȃ��Ă������ƂɋC�t����
	//MV1SetAttachAnimTime(m_model->m_handle, m_beforeAnimHandle, m_playTime);
	MV1SetAttachAnimTime(m_model->m_handle, m_nowAnimHandle, m_playTime);
}

void AnimationModel::Draw(const Vector3& drawPos) const
{
	// model��Draw�������Ɉʒu������`�ł�������
	m_model->SetPos(drawPos);
	m_model->Draw();
}

void AnimationModel::Draw() const
{
	m_model->Draw();
}
