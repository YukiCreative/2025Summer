#include "Rigid.h"
#include <algorithm>

namespace
{
	constexpr float kDontMoveThreshold = 0.0001f;
}

Rigid::Rigid() :
	m_vel(),
	m_drag(),
	m_useGravity(true)
{
}

void Rigid::Init(const float drag)
{
	m_drag = drag;
}

const Vector3 Rigid::Update()
{
	// 速度を減衰
	m_vel -= m_vel * m_drag;

	return m_vel;
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