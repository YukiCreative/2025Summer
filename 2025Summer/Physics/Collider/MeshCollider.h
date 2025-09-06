#pragma once
#include "Collider3D.h"
#include <memory>
#include <list>

class Model;

class MeshCollider : public Collider3D
{
public:
	MeshCollider();
	~MeshCollider();

	void Init(std::shared_ptr<Model> model, const int weight, const bool isThrough, const bool isStatic);

	void Draw() const override;

	// メッシュの当たり判定は何もしないと最初の形から変わらない
	// 例えばアニメーションとかを実行してメッシュの形状が変わった時はこれで反映させる
	void UpdateCol() const;

	// 当たり判定に使う要素のゲッタ
	int GetModelHadle() const;
	int GetCollisionFrameIndex() const;

private:
	std::shared_ptr<Model> m_model;
	int m_collisionFrameIndex;
};