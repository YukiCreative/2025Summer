#pragma once
#include "Collidable.h"

// 物理なし当たり判定もなし
class NoCollidable : public Collidable
{
public:
	NoCollidable();
private:
};

