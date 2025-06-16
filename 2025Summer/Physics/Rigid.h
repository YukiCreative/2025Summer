#pragma once
#include "Vector3.h"
#include "RangeLimitedValue.h"

// 物理挙動のデータをまとめたやつ
struct PhysicalMaterial
{
	// 0~1までのfloat型を宣言する
	using PhysicalParameter = RangeLimitedValue<float, 0.0f, 1.0f>;

	PhysicalMaterial(const float _groundDrag, const float _airDrag, const float _bounceFactor) :
		groundDrag(_groundDrag),
		airDrag(_airDrag),
		bounceFactor(_bounceFactor)
	{

	}

	PhysicalMaterial() = default;

	// 速度の減衰に使う　地上の抵抗(摩擦)
	// 値が大きいほど加速しにくい
	PhysicalParameter groundDrag;
	// 空気抵抗
	// 重力とかジャンプとか
	PhysicalParameter airDrag;
	// 反発係数っぽいやつ
	PhysicalParameter bounceFactor;
};

// Actorの移動速度に関する処理
// Physicsはこれを参照する
// 名前については改善の余地あり
class Rigid
{
public:
	Rigid();
	void Init(const PhysicalMaterial& material);

	// 更新
	// 速度減衰とか
	const Vector3 Update();

	void ChangeStateGround();
	void ChangeStateAir();

	// 速度に加算
	void AddVel(const Vector3& addVel);
	void Stop();
	const Vector3 GetVel()    const;
	const float   GetBounce() const; 
	// 止まっているか
	const bool    IsStop()    const;
	// 押し戻しの時は速度をいじること。
	void SetVel(const Vector3& setVel);
	// 今の移動量をｔ倍する
	void MultVel(const float t);
	// 速度のY成分を消す　それだけ
	void StopY();
	void SetGroundDrag(const float drag);
	void SetAirDrag(const float drag);
private:
	// 今フレームの地上移動量
	Vector3 m_vel;
	// 物理挙動に使うデータをまとめた
	// これで関数の引数がいちいち変わらないで済む
	PhysicalMaterial m_material;

	using UpdateFunc_t = const Vector3 (Rigid::*)();
	UpdateFunc_t m_update;
private:
	const Vector3 GroundUpdate();
	const Vector3 AirUpdate();
};

