#pragma once
#include "Collider3D.h"
#include <memory>
#include <list>

namespace DxLib
{
	struct tagMV1_COLL_RESULT_POLY_DIM;
	struct tagMV1_REF_POLYGONLIST;
}

class PolygonCollider : public Collider3D
{
public:
	PolygonCollider();
	~PolygonCollider();

	void Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const int handle, const bool useTransform);

	// DxLibの参照メッシュの更新
	// 当たり判定する時にでも呼んでくれ
	void RefreshRefMesh();
	void Draw() const override;

	const int GetModelHandle() const { return m_model; }
	void SetModelHandle(const int handle) { m_model = handle; }
	// 参照用メッシュの構造体を返します
	const DxLib::tagMV1_REF_POLYGONLIST GetRefMesh();
private:
	int m_model;
	// "CllisionFrame"というフレームの番号
	int m_frameIndex;
	// このモデルが移動したりアニメーションしたりするかどうか
	// 移動しない場合はその分軽くなると予想
	bool m_useTransform;
};