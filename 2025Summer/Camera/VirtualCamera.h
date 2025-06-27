#pragma once
#include "Vector3.h"

class Camera;

// “Á’è‚ÌˆÊ’u‚Æ’‹“_‚ÉƒJƒƒ‰‚ğ‚Á‚Ä‚­‚é
// ‚±‚ê‚ğŒp³‚µ‚Ä“Áê‚Èˆ—‚ğ‚·‚éVirtualCamera‚ğì‚Á‚Ä‚à‚æ‚¢
class VirtualCamera
{
	friend Camera;

public:

	VirtualCamera();

	void Init(const Vector3& pos, const Vector3& target);

	void SetActive(const bool isActive) { m_isActive = isActive; }

	// ‰½‚à‚µ‚È‚¢
	virtual void Update();

	const Vector3 CameraDir() const;

	const bool IsActive()    const { return m_isActive; }
	const int  GetPriority() const { return m_priority; }

	void SetPos(const Vector3& pos) { m_pos = pos; }
	void SetTarget(const Vector3& target) { m_target = target; }

protected:

	bool m_isActive;
	int m_priority;
	Vector3 m_pos;
	Vector3 m_target;
	float m_fov;

};

