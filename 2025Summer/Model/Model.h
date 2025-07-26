#pragma once
#include "Vector3.h"
#include <string>

class AnimationModel;

namespace DxLib
{
	struct tagMATRIX;
}

// 3DモデルのLoad&Deleteをクラス化したい
class Model
{
	friend AnimationModel;
public:
	Model();
	~Model();

	// ハンドルから複製
	void Init(const int duplicateHandle);
	// パスから新規読み込み
	void Init(const std::string& path);

	void SetPos(const Vector3& pos) const;
	void CommitMove(const Vector3& moveValue);
	void Draw() const;

	void RotateUpVecY(const float rad);
	void RotateUpVecZ(const float rad);
	// これを使うとMatrixによってしか座標変換できなくなるので注意
	void SetMatrix(DxLib::tagMATRIX mat);
	void SetScale(const Vector3& scale);
	DxLib::tagMATRIX GetMatrix() const;
	DxLib::tagMATRIX GetFrameMatrix(const std::string frameName) const;
	Vector3 GetFramePosition(const std::string& frameName) const;
	Vector3 GetFramePosition(const int frameIndex) const;
	const int GetFrameNum() const;
	const Vector3 GetModelBBMin() const;
	const Vector3 GetModelBBMax() const;

private:
	int m_handle;
};