#pragma once
#include "Collider3D.h"
class CapsuleCollider : public Collider3D
{
public:
	CapsuleCollider();

	void Init();

	void Draw() const override;


private:
};

