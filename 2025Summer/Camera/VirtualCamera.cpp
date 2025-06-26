#include "VirtualCamera.h"

VirtualCamera::VirtualCamera() :
	m_isActive(true),
	m_priority(0)
{
}

void VirtualCamera::Init(const Vector3& pos, const Vector3& target)
{
	m_pos = pos;
	m_target = target;
}


void VirtualCamera::Update()
{
}

const Vector3 VirtualCamera::CameraDir() const
{
	return (m_target - m_pos).GetNormalize();
}
