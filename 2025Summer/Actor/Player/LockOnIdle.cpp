#include "LockOnIdle.h"
#include "Player.h"
#include "AnimationModel.h"

LockOnIdle::LockOnIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

LockOnIdle::~LockOnIdle()
{
}

std::shared_ptr<PlayerState> LockOnIdle::Update()
{
	// ƒvƒŒƒCƒ„[‚ð“G•ûŒü‚É‰ñ“]
	auto p = m_player.lock();

	auto lockOnPosXZ = p->m_lockOnActor.lock()->GetPos().XZ();
	auto posXZ = p->GetPos().XZ();

	auto lockOnToPlayerXZ = posXZ - lockOnPosXZ;

	p->m_model->RotateUpVecY(lockOnToPlayerXZ.Cross(-p->m_model->GetDirection()).y * 0.01f);

	return shared_from_this();
}
