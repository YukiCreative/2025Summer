#pragma once
#include "../Geometry/Vector3.h"
#include <string>

class AnimationModel;
class MeshCollider;

namespace DxLib
{
	struct tagMATRIX;
}

// 3DモデルのLoad&Deleteをクラス化したい
class Model
{
	friend AnimationModel;
	friend MeshCollider;
public:
	Model();
	~Model();

	// ハンドルから複製
	void Init(const int duplicateHandle);
	// パスから新規読み込み
	void Init(const std::string& path);

	void SetPos(const Vector3& pos) const;
	const Vector3 GetPos() const;
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
	void SetWriteZBuffer(const bool writeOrNot);

private:
	int m_handle;
};