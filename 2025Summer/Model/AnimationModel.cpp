#include "AnimationModel.h"
#include <DxLib.h>
#include "Model.h"
#include <cassert>
#include "../General/Game.h"

void AnimationModel::ChangeAnimation(const int animId, bool isLoop)
{
	m_isLoop = isLoop;
	m_isPlaying = true;

	// まず二つ前のアニメーションをデタッチ
	MV1DetachAnim(m_model->m_handle, m_beforeAnimHandle);

	// ひとつ前のアニメーションを入れとく
	m_beforeAnimHandle = m_nowAnimHandle;

	// 新しいのをアタッチ
	m_nowAnimHandle = MV1AttachAnim(m_model->m_handle, animId);
	assert(m_nowAnimHandle != -1 && "アニメーションのアタッチに失敗した");

	m_playTime = 0;

	// アニメーションの時間を把握しておく
	m_animTotalTime = MV1GetAttachAnimTotalTime(m_model->m_handle, m_nowAnimHandle);

	// アニメーションのブレンド率を設定
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_beforeAnimHandle, 1.0f);
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_nowAnimHandle, 0.0f);

	// ブレンド率をリセット
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

	// アニメーションの時間を進める
	// 多分単位はミリ秒
	m_playTime += m_playSpeed / Game::kFrameRate;

	if (m_playTime > m_animTotalTime)
	{
		// ループしていいならループ
		if (m_isLoop)
		{
			m_playTime -= m_animTotalTime;
		}
		else
		{
			m_isPlaying = false;
		}
	}

	// ブレンド率を設定
	m_blendRate += m_animBlendSpeed;

	// アニメーションのブレンド率を設定
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_beforeAnimHandle, 1 - m_blendRate.Value());
	MV1SetAttachAnimBlendRate(m_model->m_handle, m_nowAnimHandle, m_blendRate.Value());

	// 元のアニメーションは進めなくていいことに気付いた
	//MV1SetAttachAnimTime(m_model->m_handle, m_beforeAnimHandle, m_playTime);
	MV1SetAttachAnimTime(m_model->m_handle, m_nowAnimHandle, m_playTime);
}

void AnimationModel::Draw(const Vector3& drawPos) const
{
	// modelのDrawも引数に位置を入れる形でいいかも
	m_model->SetPos(drawPos);
	m_model->Draw();
}

void AnimationModel::Draw() const
{
	m_model->Draw();
}
