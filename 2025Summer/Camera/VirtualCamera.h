#pragma once
#include "Vector3.h"

class Camera;

// 特定の位置と注視点にカメラを持ってくる
// これを継承して特殊な処理をするVirtualCameraを作ってもよい
class VirtualCamera
{
	// 派生クラスで追加する変数は少ないと判断
	friend Camera;

public:

	VirtualCamera();

	void Init(const Vector3& pos, const Vector3& target);

	void SetActive(const bool isActive) { m_isActive = isActive; }

	// 何もしない
	virtual void Update();

	const Vector3 CameraDir() const;

	const bool IsActive()    const { return m_isActive; }
	const int  GetPriority() const { return m_priority; }

private:

	bool m_isActive;
	int m_priority;
	Vector3 m_pos;
	Vector3 m_target;

};

