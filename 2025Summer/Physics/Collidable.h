#pragma once
#include <memory>
#include "../Geometry/Vector3.h"

class Collider3D;
class Rigid;

enum class ColKind3D;

// Collider3DとRigidまとめた
class Collidable
{
public:
	Collidable();

	// 外で生成してこれにぶち込む
	void Init(std::shared_ptr<Collider3D> col, std::shared_ptr<Rigid> rigid);

	Vector3 UpdateRigid();

	// Collider,Rigidの変数をそれらを経由せずに取得できる
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

