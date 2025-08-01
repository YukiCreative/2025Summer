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

// 3Dモデルをいい感じに使いやすくアニメーションさせる
// Modelを継承してしまうか考え中
class AnimationModel
{
public:
	AnimationModel();
	~AnimationModel();

	void Init(const std::string& path, const float animSpeed);
	// すでにあるハンドルを参照するパターン
	// デストラクタでモデルを削除しない
	void Init(const int handle, const float animSpeed);

	void Update();
	void Draw(const Vector3& drawPos) const;
	void Draw() const;

	void Stop() { m_isPlaying = false; }
	void Play() { m_isPlaying = true; }
	const bool IsPlaying() const { return m_isPlaying; }
	// アニメーションが終わっているかどうか
	const bool IsEnd() const { return !m_isPlaying; }
	// アニメーションのブレンドができます！
	void ChangeAnimation(const int animId, bool isLoop = true);
	void ChangeAnimation(const std::string& animName, bool isLoop = true);
	// おそらくModelViewerの時間単位
	int GetAnimTotalTime() const;

	// この辺Modelクラスの中継
	// Y軸を基準に回転
	void RotateUpVecY(const float rad);
	void RotateUpVecZ(const float rad);
	void SetPos(const Vector3& pos);
	DxLib::tagMATRIX GetMatrix() const;
	void SetMatrix(const DxLib::tagMATRIX& mat);
	// 今のアニメーションを確かめるときに使う
	const bool CheckAnimName(const std::string name);
	// モデルの行列のZ基底ベクトルだったものを返す
	// プレイヤーのモデルが反転していたので、それに合わせて符号を反転してしまっているので注意(今更全部変更できない…)
	const Vector3 GetDirection();
	// 0～1
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

