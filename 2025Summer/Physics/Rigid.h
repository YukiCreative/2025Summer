#pragma once
#include "Vector3.h"
#include "RangeLimitedValue.h"

// Actorの移動速度に関する処理
// Physicsはこれを参照する
// 名前については改善の余地あり
class Rigid
{
public:
	Rigid();
	void Init(const float drag);

	// 更新
	// 速度減衰とか
	const Vector3 Update();

	void ChangeStateGround();
	void ChangeStateAir();

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
private:
	// 今フレームの地上移動量
	Vector3 m_vel;
	// 速度減衰
	float m_drag;

	bool m_useGravity;

	using UpdateFunc_t = const Vector3 (Rigid::*)();
	UpdateFunc_t m_update;
};

