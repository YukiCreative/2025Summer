#pragma once
#include "Collidable.h"

// •¨—‚È‚µ“–‚½‚è”»’è‚à‚È‚µ
class NoCollidable : public Collidable
{
public:
	NoCollidable();

	void Init(std::shared_ptr<Collider3D> col, std::shared_ptr<Rigid> rigid) = delete;
private:
};

