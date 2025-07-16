#include "Actor.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "CollisionChecker.h"
#include "Geometry.h"
#include "Rigid.h"
#include <algorithm>
#include <array>
#include <DxLib.h>

float CollisionChecker::WeightRate(Collidable& colA, Collidable& colB)
{
	float weightRate = static_cast<float>(colB.GetWeight()) / (static_cast<float>(colA.GetWeight()) + static_cast<float>(colB.GetWeight())); // ���̂��󂯂�ړ��ʂ̊���

	// ����A��B�ǂ��炩��static�Ȃ�A�����͓�������
	// �������ɂ��ׂĂ������t����
	if (colA.IsStatic())
	{
		weightRate = 0.0f;
	}
	else if (colB.IsStatic())
	{
		weightRate = 1.0f;
	}
	// �ǂ�����static�Ȃ����������
	if (colA.IsStatic() && colB.IsStatic())
	{
		colA.SetVel(Vector3::Zero());
		colB.SetVel(Vector3::Zero());
	}

	return weightRate;
}

bool CollisionChecker::CheckHitSS(const Collidable& colA, const Collidable& colB)
{
	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	// �ړ���̈ʒu�𒲂ׂ�
	const auto nextA = colA.GetPos() + colA.GetVel();
	const auto nextB = colB.GetPos() + colB.GetVel();

	const auto radiusSum = sphereColA.GetRadius() + sphereColB.GetRadius();

	return (nextA - nextB).SqrMagnitude() < radiusSum * radiusSum;
}

void CollisionChecker::FixMoveSS(Collidable& colA, Collidable& colB)
{
	// �ړ���̈ʒu
	const auto nextA = colA.GetPos() + colA.GetVel();
	const auto nextB = colB.GetPos() + colB.GetVel();

	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	const auto radiusSum = sphereColA.GetRadius() + sphereColB.GetRadius();

	const auto atoBN = (nextB - nextA).GetNormalize();

	// �߂荞�񂾕������
	const float overlapLength = radiusSum - (nextA - nextB).Magnitude();

	const Vector3 overlap = atoBN * overlapLength;

	const auto weightRate = WeightRate(colA, colB);

	// �Z�o�����l�𔽉f
	colA.AddVel(-overlap * weightRate);
	colB.AddVel(overlap * (1 - weightRate));
}

void CollisionChecker::ComparePolyHit(PolyHitData& a, const PolyHitData& b)
{
	if (a.time < b.time) return;

	a = b;
}

bool CollisionChecker::CheckHitSP(const Collidable& sCol, const Collidable& pCol, PolyHitData& hit)
{
	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(pCol.GetCol());

	bool isHit = false;

	const Vector3 sumVel = sCol.GetVel() - pCol.GetVel(); // ���̂��猩�����Α��x�ōl����

	// �܂��A�|���S���̃��b�V���̏�Ԃ��擾����
	// ���߂̍X�V����
	polygonCol.RefreshRefMesh();
	auto& refMesh = polygonCol.GetRefMesh();

	// �擾�����|���S�����ׂĂɑ΂��Ď��s(��΂���)
	for (int i = 0; i < refMesh.PolygonNum; ++i)
	{
		// �Ƃ肠�����K�v�ȏ����擾
		const auto& polygon = refMesh.Polygons[i];
		const auto& vertices = refMesh.Vertexs;
		// ���_�̍��W
		const std::array<Vector3, 3> polygonVertices =
		{
			vertices[polygon.VIndex[0]].Position,
			vertices[polygon.VIndex[1]].Position,
			vertices[polygon.VIndex[2]].Position,
		};
		DrawSphere3D(polygonVertices[0], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[1], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[2], 100, 10, 0xff0000, 0xff0000, true);
		// ��
		const std::array<Vector3, 3> polygonLines =
		{
			polygonVertices[1] - polygonVertices[0], // 0~1�Ɍ������x�N�g��
			polygonVertices[2] - polygonVertices[1], // 1~2
			polygonVertices[0] - polygonVertices[2], // 2~0
		};
		// (�ʂ�)�@��
		const Vector3 normal = polygonLines[2].Cross(polygonLines[0]).GetNormalize();
		DrawLine3D(pCol.GetPos(), pCol.GetPos() + normal * 500, 0xffffff);
		const float dot = sumVel.Dot(normal); // ���܂̈ړ������Ɩʂ̌����̊֌W���ǂꂮ�炢�����킩��
		const Vector3 c0 = sphereCol.GetPos() - polygonVertices[0]; // �ǂ����K���ȃ|���S���̒��_���狅�̌��݈ʒu�܂�
		const float dotC0 = c0.Dot(normal);
		const float distPlaneToPoint = fabsf(dotC0); // �ʂ���_�ւ̋���

		const float collideTime = (sphereCol.GetRadius() - dotC0) / dot; // ���ꂪ�Փˎ���

		if (dot >= 0) continue; // �ړ��������ʂɑ΂��ė����
		if (collideTime > 1 || collideTime < 0) continue; // ����̈ړ��ŏՓ˂��Ȃ��ꍇ

		// �Փ˂���
		// ���߂��Փ˂������ꍇ�̂ݍX�V
		ComparePolyHit(hit, PolyHitData(collideTime, polygonVertices, normal));
		//isHit = true;
	}

	return isHit;
}

void CollisionChecker::FixMoveSP(Collidable& _sphereCol, Collidable& _polygonCol, const PolyHitData& hitData)
{
	auto& sphereCol = static_cast<SphereCollider&>(_sphereCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(_polygonCol.GetCol());

	// �ꏊ���Œ肵�Ă݂�
	_sphereCol.SetPos(_sphereCol.GetPos() + _sphereCol.GetVel() * hitData.time);
	_polygonCol.SetPos(_polygonCol.GetPos() + _polygonCol.GetVel() * hitData.time);

	// �o�E���h
	_sphereCol.GetRigid().SetVel(_sphereCol.GetVel() * (hitData.time - 1));
}

bool CollisionChecker::CheckHitCS(const Collidable& cCol, const Collidable& sCol)
{
	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& capsuleCol = static_cast<CapsuleCollider&>(cCol.GetCol());

	// ���̒��S�ƃJ�v�Z�������Ƃ̍ŋߐړ_���o��

	const Vector3 sphereNextPos = sphereCol.GetPos() + sCol.GetRigid().GetVel();
	const Vector3 capsuleNextStartPos = capsuleCol.StartPos() + cCol.GetRigid().GetVel();

	const Vector3 startToSphere = sphereNextPos - capsuleNextStartPos;
	const Vector3 capsuleDir = capsuleCol.Direction(); // �����͈ړ��ʂ��܂�ł��ς��񂾂�

	float projection = startToSphere.Dot(capsuleDir);

	// projection������̒����܂łɐ���
	projection = std::clamp(projection, 0.0f, capsuleCol.Length());

	const Vector3 nearestPosOnLine = capsuleNextStartPos + capsuleDir * projection;

	const float radiusSum = sphereCol.GetRadius() + capsuleCol.GetRadius();

	// ���̒��S���ŋߐړ_�����ꂼ��̔��a�̍��v���Z����Γ������Ă�
	return (nearestPosOnLine - sphereCol.GetPos()).SqrMagnitude() < radiusSum * radiusSum;
}

void CollisionChecker::FixMoveCS(Collidable& cCol, Collidable& sCol)
{
	// ������Ȃ��ʒu�܂ő��x��␳


	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& capsuleCol = static_cast<CapsuleCollider&>(cCol.GetCol());

	// ���̒��S�ƃJ�v�Z�������Ƃ̍ŋߐړ_���������o��

	const Vector3 sphereNextPos = sphereCol.GetPos() + sCol.GetRigid().GetVel();
	const Vector3 capsuleNextStartPos = capsuleCol.StartPos() + cCol.GetRigid().GetVel();

	const Vector3 startToSphere = sphereNextPos - capsuleNextStartPos;
	const Vector3 capsuleDir = capsuleCol.Direction(); // �����͈ړ��ʂ��܂�ł��ς��񂾂�

	float projection = startToSphere.Dot(capsuleDir);

	// projection������̒����܂łɐ���
	projection = std::clamp(projection, 0.0f, capsuleCol.Length());

	const Vector3 nearestPosOnLine = capsuleNextStartPos + capsuleDir * projection;

	// �߂荞��ł���x�N�g�����ق���

	auto sphereToNearest = nearestPosOnLine - sphereNextPos;
	auto sphereToNearestN = sphereToNearest.GetNormalize();

	auto sphereToNearestLength = sphereToNearest.Magnitude();

	auto radiusSum = capsuleCol.GetRadius() + sphereCol.GetRadius();

	auto diff = radiusSum - sphereToNearestLength;

	const Vector3 overlap = sphereToNearestN * diff;

	DrawLine3D({0,0,0}, overlap, 0xff0000);
	DrawSphere3D(nearestPosOnLine, 10, 10, 0xffffff, 0xffffff, true);

	// �߂荞�񂾃x�N�g�����A���ꂼ��̏d���̔䗦�ŕ��z

	float weightRate = static_cast<float>(cCol.GetWeight()) / (static_cast<float>(cCol.GetWeight()) + static_cast<float>(sCol.GetWeight())); // ���̂��󂯂�ړ��ʂ̊���

	// ����A��B�ǂ��炩��static�Ȃ�A�����͓�������
	// �������ɂ��ׂĂ������t����
	if (cCol.IsStatic())
	{
		weightRate = 1.0f;
	}
	else if (sCol.IsStatic())
	{
		weightRate = 0.0f;
	}
	// �ǂ�����static�Ȃ����������
	if (cCol.IsStatic() && sCol.IsStatic())
	{
		cCol.SetVel(Vector3::Zero());
		sCol.SetVel(Vector3::Zero());
		return;
	}

	cCol.AddVel(overlap * (1.0f - weightRate));
	sCol.AddVel(-overlap * weightRate);

}

bool CollisionChecker::CheckHitCC(const Collidable& colA, const Collidable& colB)
{
	auto& cColA = static_cast<CapsuleCollider&>(colA.GetCol());
	auto& cColB = static_cast<CapsuleCollider&>(colB.GetCol());

	const auto velA = colA.GetVel();
	const auto velB = colB.GetVel();

	// DxLib���g�킹�Ă�������
	return HitCheck_Capsule_Capsule(cColA.StartPos() + velA, cColA.EndPos() + velA, cColA.GetRadius(), cColB.StartPos() + velB, cColB.EndPos() + velB, cColB.GetRadius());
}

void CollisionChecker::FixMoveCC(Collidable& colA, Collidable& colB)
{
	// �ŋߐړ_�����ꂼ�ꒆ�S�Ɏ��������̉����߂�

	auto& cColA = static_cast<CapsuleCollider&>(colA.GetCol());
	auto& cColB = static_cast<CapsuleCollider&>(colB.GetCol());

	float t, s;
	Vector3 minPos1, minPos2;

	const Vector3 velA       = colA.GetVel();
	const Vector3 velB       = colB.GetVel();
	const Vector3 nextPosA   = cColA.GetPos()   + velA;
	const Vector3 nextStartA = cColA.StartPos() + velA;
	const Vector3 nextEndA   = cColA.EndPos()   + velA;
	const Vector3 nextPosB   = cColB.GetPos()   + velB;
	const Vector3 nextStartB = cColB.StartPos() + velB;
	const Vector3 nextEndB   = cColB.EndPos()   + velB;

	// ���K������Ă��Ȃ��������K�v
	const Vector3 dirA = nextEndA - nextPosA;
	const Vector3 dirB = nextEndB - nextPosB;

	const Vector3 deltaPos = nextPosB - nextPosA;
	const Vector3 dirNormal = dirA.Cross(dirB);

	// �J�v�Z�������s���ǂ���
	if (dirNormal.SqrMagnitude() < 0.01f)
	{
		// ���s

		// A�̓K���Ȓ[�_����A��ԋ߂�B�̓_�����߂�
		// ��������A��ԋ߂�A�̓_�����߂�
		// �o����̓_���ŋߐړ_

		minPos2 = Geometry::PointSegmentNearestPos(nextStartA, nextStartB, nextEndB);

		minPos1 = Geometry::PointSegmentNearestPos(minPos2, nextStartA, nextEndA);
	}
	else
	{
		// ���s�łȂ�

		// �s��ŘA����������������炵��
		MATRIX matSolve = MGetAxis1
		(
			 dirA,
			-dirB,
			dirNormal,
			{0,0,0}
		);

		matSolve = MInverse(matSolve);

		s = Vector3{ matSolve.m[0][0],matSolve.m[1][0] ,matSolve.m[2][0] }.Dot(deltaPos);
		t = Vector3{ matSolve.m[0][1],matSolve.m[1][1] ,matSolve.m[2][1] }.Dot(deltaPos);

		// clamp
		s = std::clamp(s, -1.0f, 1.0f);
		t = std::clamp(t, -1.0f, 1.0f);

		minPos1 = nextPosA + dirA * s;
		minPos2 = nextPosB + dirB * t;

		// �ŋߐړ_���[�ɂȂ������̋������������������̂ŏC��
		if (s <= -0.9999f || s >= 0.9999f)
		{
			// �v�Z���Ȃ���
			minPos2 = Geometry::PointSegmentNearestPos(minPos1, nextStartB, nextEndB);
		}
		if (t <= -0.9999f || t >= 0.9999f)
		{
			minPos1 = Geometry::PointSegmentNearestPos(minPos2, nextStartA, nextEndA);
		}
	}

	DrawSphere3D(minPos1, 10,10, 0xffffff,0xffffff, true);
	DrawSphere3D(minPos2, 10,10, 0xffffff,0xffffff, true);

	// ��͋��̂̉����߂��Ɠ���
	// �߂荞��ł���x�N�g�����ق���

	const Vector3 nearestPosDiff = minPos2 - minPos1;
	const Vector3 nearestPosDiffN = nearestPosDiff.GetNormalize();

	const float radiusSum = cColA.GetRadius() + cColB.GetRadius();
	const float rerativeLength = nearestPosDiff.Magnitude();
	const float diff = radiusSum - rerativeLength;

	const Vector3 overlap = nearestPosDiffN * diff;

	const float weightRate = WeightRate(colA, colB);

	colA.AddVel(-overlap * weightRate);
	colB.AddVel(overlap * (1.0f - weightRate));
}

// ===============================================

PolyHitData::PolyHitData() :
	time(kBigNum),
	vertices(),
	normal()
{
}

PolyHitData::PolyHitData(const float _time, const Vertices& _vertices, const Vector3& _normal)
{
	time = _time;
	vertices = _vertices;
	normal = _normal;
}

void PolyHitData::operator=(const PolyHitData& other)
{
	time = other.time;
	vertices = other.vertices;
	normal = other.normal;
}