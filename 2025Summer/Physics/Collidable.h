#pragma once
#include <memory>
#include "Vector3.h"

class Collider3D;
class Rigid;

enum class ColKind3D;

// Collider3D‚ÆRigid‚Ü‚Æ‚ß‚½
class Collidable
{
public:
	Collidable();

	// ŠO‚Å¶¬‚µ‚Ä‚±‚ê‚É‚Ô‚¿‚Ş
	void Init(std::shared_ptr<Collider3D> col, std::shared_ptr<Rigid> rigid);

	Vector3 UpdateRigid();

	// Collider,Rigid‚Ì•Ï”‚ğ‚»‚ê‚ç‚ğŒo—R‚¹‚¸‚Éæ“¾‚Å‚«‚é
	const Vector3   GetVel()     const;
	const ColKind3D GetColKind() const;
	const bool      IsThrough()  const;
	const bool      IsStatic()   const;
	const bool      IsStop()     const;
	const int       GetWeight()  const;
	const Vector3   GetPos()     const;

	void AddVel(const Vector3& vel);
	void SetVel(const Vector3& vel);
	void SetPos(const Vector3& pos);

	bool HasCol() const;
	Collider3D& GetCol() const { return *m_col; }
	bool HasRigid() const;
	Rigid& GetRigid() const { return *m_rigid; }
private:
	std::shared_ptr<Collider3D> m_col;
	std::shared_ptr<Rigid> m_rigid;
};

