#include "RotateCamera.h"
#include "Input.h"
#include <DxLib.h>

namespace
{
	constexpr float kCameraDrag = 0.15f;

	constexpr float kCameraVRotPlusThreshold = 0.3f;
	constexpr float kCameraVRotMinusThreshold = 1.0f;

	constexpr float kDistanceEasingStrength = 0.1f;
}

RotateCamera::RotateCamera() :
	m_lerpedCameraDistance(100),
	m_cameraDistance(100)
{
}

void RotateCamera::Init(const Vector3& target, const Vector3& targetToCamera, const float cameraDistance)
{
	// •K—v‚È•Ï”‚ğ“ü‚ê‚é‚¾‚¯
	m_target = target;
	m_targetToCamera = targetToCamera;
	m_cameraDistance = cameraDistance;
}

void RotateCamera::Update()
{
	Rotate();

	EasingDistance();

	// “K—p
	m_pos = m_target + m_targetToCamera * m_lerpedCameraDistance;
}

void RotateCamera::AddVel(const Vector2& vel)
{
	m_vel += vel;
}

void RotateCamera::SetVel(const Vector2& vel)
{
	m_vel = vel;
}

void RotateCamera::SetCameraDistance(const float dist)
{
	m_cameraDistance = dist;
}

void RotateCamera::Rotate()
{
	// ƒJƒƒ‰‘¬“xŒ¸Š
	m_vel -= m_vel * kCameraDrag;

	// …•½‰ñ“]
	m_targetToCamera = VTransformSR(m_targetToCamera, MGetRotY(m_vel.x));

	// ‚’¼‰ñ“]
	// ‚Ü‚¸‰ñ“]²‚ğè‚É“ü‚ê‚é
	const MATRIX camMat = MTranspose(GetCameraViewMatrix());
	// ƒJƒƒ‰‚É‚Æ‚Á‚Ä‚ÌX²@‚ğæ“¾‚µ‚Ä‚¢‚é
	const Vector3 rightAxis = { camMat.m[0][0],camMat.m[0][1],camMat.m[0][2] };

	// ˆê’èˆÈã‹}‚ÈŠp“x‚É‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚é
	// ‰ñ“]‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚éŒü‚«‚ªAŠp“x‚ğŒ³‚É–ß‚·•ûŒü‚È‚ç‹–‚·
	const Vector3 unitTargetToCamera = m_targetToCamera.GetNormalize();
	const Vector3 unitTargetToCameraXZ = { unitTargetToCamera.x, 0, unitTargetToCamera.z };
	const float dot = unitTargetToCamera.Dot(unitTargetToCameraXZ);

	if ((dot < kCameraVRotPlusThreshold && m_vel.y > 0 && m_targetToCamera.y > 0) ||
		(dot < kCameraVRotMinusThreshold && m_vel.y < 0 && m_targetToCamera.y < 0)
		)
	{
		m_vel.y = 0;
		return;
	}

	// ‰ñ“]²‚É‚»‚Á‚Ä‰ñ“]
	m_targetToCamera = VTransform(m_targetToCamera, MGetRotAxis(rightAxis, m_vel.y));
}

void RotateCamera::EasingDistance()
{
	m_lerpedCameraDistance = std::lerp(m_lerpedCameraDistance, m_cameraDistance, kDistanceEasingStrength);
}
