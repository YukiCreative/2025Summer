#include "Rigid.h"
#include <algorithm>

namespace
{
	constexpr float kDontMoveThreshold = 0.0001f;
}

Rigid::Rigid() :
	m_vel(),
	m_material(),
	m_update(&Rigid::GroundUpdate),
	m_useGravity(true)
{
}

void Rigid::Init(const PhysicalMaterial& material)
{
	m_material = material;
}

const Vector3 Rigid::Update()
{
	return (this->*m_update)();
}

void Rigid::ChangeStateGround()
{
	m_update = &Rigid::GroundUpdate;
}

void Rigid::ChangeStateAir()
{
	m_update = &Rigid::AirUpdate;
}

void Rigid::AddVel(const Vector3& addVel)
{
	m_vel += addVel;
}

void Rigid::Stop()
{
	m_vel = Vector3::Zero();
}

const Vector3 Rigid::GetVel() const
{
	return m_vel;
}

const float Rigid::GetBounce() const
{
	return m_material.bounceFactor.Value();
}

const bool Rigid::IsStop() const
{
	// 止まっているかどうか
	// 完全に0にはならないので、遊びを設ける
	return fabsf(m_vel.x) < kDontMoveThreshold &&
		   fabsf(m_vel.y) < kDontMoveThreshold &&
		   fabsf(m_vel.z) < kDontMoveThreshold;
}

void Rigid::SetVel(const Vector3& setVel)
{
	m_vel = setVel;
}

void Rigid::MultVel(const float t)
{
	m_vel *= t;
}

void Rigid::StopY()
{
	m_vel.y = 0;
}

void Rigid::SetGroundDrag(const float drag)
{
	m_material.groundDrag = drag;
}

void Rigid::SetAirDrag(const float drag)
{
	m_material.airDrag = drag;
}

const Vector3 Rigid::GroundUpdate()
{
	// 速度を減衰
	m_vel -= m_vel * m_material.groundDrag.Value();

	return m_vel;
}

const Vector3 Rigid::AirUpdate()
{
	// 空気抵抗
	m_vel -= m_vel * m_material.airDrag.Value();

	return m_vel;
}