#pragma once
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector2.h"
#include "../Utility/RangeLimitedValue.h"

// Actorの移動速度に関する処理
// Physicsはこれを参照する
// 名前については改善の余地あり
class Rigid
{
public:
	Rigid();
	void Init(const Vector2& drag);

	// 更新
	// 速度減衰とか
	const Vector3 Update();

	// 速度に加算
	void AddVel(const Vector3& addVel);
	void Stop();
	const Vector3 GetVel()    const;
	// 止まっているか
	const bool    IsStop()    const;
	// 押し戻しの時は速度をいじること。
	void SetVel(const Vector3& setVel);
	// 今の移動量をｔ倍する
	void MultVel(const float t);
	// 速度のY成分を消す　それだけ
	void StopY();
	void SetUseGravity(const bool useOrNot) { m_useGravity = useOrNot; }
	bool IsUseGravity() const { return m_useGravity; }
	const Vector2& GetDrag() const { return m_drag; }
	void SetDrag(const Vector2& drag) { m_drag = drag; }
	// 初期化時の抵抗に戻す
	void SetDragDefault() { m_drag = m_defaultDrag; }
	// 今のY抵抗が初期値より小さいとtrue
	bool IsMinDefaultDragY() const { return m_drag.y < m_defaultDrag.y; }
	void SetGravityMagnification(float magnification) { m_gravityMagnification = magnification; }
	float GetGravityMagnification() const { return m_gravityMagnification; }

private:
	// 今フレームの地上移動量
	Vector3 m_vel;
	// 速度減衰
	Vector2 m_drag;
	Vector2 m_defaultDrag;
	float m_gravityMagnification = 1.0f;

	bool m_useGravity;
};

