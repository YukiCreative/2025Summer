#pragma once
#include <memory>
#include <unordered_map>
#include "Vector3.h"

enum class ColKind3D
{
	kNone, // Noneはエラーを吐くための属性
	kSphere,
	kBox,
	kPolygon,
	kCapsule,
};

// 当たり判定クラス
// 衝突に必要な情報を持つ
class Collider3D abstract
{
public:
	Collider3D();
	virtual ~Collider3D();

	void Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic);

	virtual void Draw() const abstract;

	const ColKind3D GetColKind() const { return m_colKind; }
	virtual void SetPos(const Vector3& pos);
	const Vector3& GetPos() const { return m_pos; }
	const int  GetWeight() const { return m_weight; }
	const bool IsThrough() const { return m_isThrough; }
	const bool IsStatic()  const { return m_isStatic; }

	void SetIsThrough(const bool isThrough) { m_isThrough = isThrough; }
	void SetIsStatic(const bool isStatic) { m_isStatic = isStatic; }

	bool IsValid() const { return m_isValid; }
	void ValidCol() { m_isValid = true; }
	void InvalidCol() { m_isValid = false; }

protected:
	ColKind3D m_colKind;
	Vector3 m_pos;
	// このフレームに動いたかどうか
	bool m_isMove;
	// そもそも押し戻し処理を行うかどうか
	// どちらかがIsThroughなら行わない
	bool m_isThrough;
	// 押し戻しされるかどうか
	bool m_isStatic;
	// 重さ
	// 当たった対象との押し戻しの配分に使う
	int m_weight;
	// 有効かどうか
	bool m_isValid;
};