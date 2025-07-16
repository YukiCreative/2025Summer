#include "NoCollidable.h"
#include "SphereCollider.h"
#include "Rigid.h"

NoCollidable::NoCollidable()
{
	// 無効化したコライダーを入れておく
	auto col = std::make_shared<SphereCollider>();
	col->Init(Vector3::Zero(), 0, true, true, 0);
	col->InvalidCol();
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(0.0f);

	Collidable::Init(col, rigid);
}
