#include "Collidable.h"
#include "Collider3D.h"
#include "Rigid.h"

Collidable::Collidable()
{
}

void Collidable::Init(std::shared_ptr<Collider3D> col, std::shared_ptr<Rigid> rigid)
{
	m_col = col;
	m_rigid = rigid;
}

Vector3 Collidable::UpdateRigid()
{
	return m_rigid->Update();
}

const Vector3 Collidable::GetVel() const
{
	return m_rigid->GetVel();
}

const ColKind3D Collidable::GetColKind() const
{
	return m_col->GetColKind();
}

const bool Collidable::IsThrough() const
{
	return m_col->IsThrough();
}

const bool Collidable::IsStatic() const
{
	return m_col->IsStatic();
}

const bool Collidable::IsStop() const
{
	return m_rigid->IsStop();
}

const int Collidable::GetWeight() const
{
	return m_col->GetWeight();
}

const Vector3 Collidable::GetPos() const
{
	return m_col->GetPos();
}

void Collidable::AddVel(const Vector3& vel)
{
	m_rigid->AddVel(vel);
}

void Collidable::SetVel(const Vector3& vel)
{
	m_rigid->SetVel(vel);
}

void Collidable::SetPos(const Vector3& pos)
{
	m_col->SetPos(pos);
}

bool Collidable::HasCol() const
{
	return static_cast<bool>(m_col);
}

bool Collidable::HasRigid() const
{
	return static_cast<bool>(m_rigid);
}
