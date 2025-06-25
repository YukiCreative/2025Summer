#include "Physics.h"
#include "Actor.h"
#include "Collider3D.h"
#include "SphereCollider.h"
#include "PolygonCollider.h"
#include "CollisionChecker.h"
#include "Collidable.h"
#include "Rigid.h"

namespace
{
	// �d�͂͒萔
	// ��ŕϐ��ɂ��邩���H�H�H
	const Vector3 kGravity = { 0, -0.8f, 0 };

	constexpr int kCheckLoopMax = 1;
}

void Physics::Update(std::list<std::shared_ptr<Actor>> actorList)
{
	// �d��
	Gravity(actorList);

	CheckHit(actorList);
}

void Physics::CheckHit(std::list<std::shared_ptr<Actor>>& actorList)
{
	bool isHit = true;
	int loopCount = 0;

	// ������Ȃ��Ȃ邩�A���̉񐔃��[�v����܂ŏ������p��
	while (isHit && loopCount < kCheckLoopMax)
	{
		isHit = false;

		for (auto& actA : actorList)
		{
			for (auto& actB : actorList)
			{
				if (!actA->CanCollide() || !actB->CanCollide()) continue;

				// ����l���Ȃ�v�Z���Ȃ�
				if (actA == actB) continue;

				Collidable& colA = actA->GetCollidable();
				Collidable& colB = actB->GetCollidable();

				// �ǂ�������t���[���ɓ����Ă��Ȃ���Γ������Ă��Ȃ�
				if (colA.IsStop() && colB.IsStop()) continue;

				const ColKind3D colKindA = colA.GetColKind();
				const ColKind3D colKindB = colB.GetColKind();

				// ���蔲���邩�ǂ���
				const bool skipPushBack = colA.IsThrough() || colB.IsThrough();
				bool hitResult = false;

				// ���Collider�̎�ނɉ����������蔻��֐����Ă�
				if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kSphere)
				{
					float time;
					hitResult = CollisionChecker::CheckHitSS(colA, colB, time);

					if (hitResult)
					{
						// �����߂�����
						// ���ߐݒ���m�F���āA�ǂ�����s���߂Ȃ牟���߂�
						if (!skipPushBack)
						{
							// �����߂�
							CollisionChecker::FixMoveSS(colA, colB, time);
						}
					}
				}
				else if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kPolygon)
				{
					PolyHitData hitData;
					// ���ꔽ�΂̃P�[�X���񋓂��Ȃ��Ƃ����Ȃ��̃S�~�R�[�h�߂���H
					hitResult = CollisionChecker::CheckHitSP(colA, colB, hitData);

					if (hitResult)
					{
						// �����߂�
						if (!skipPushBack)
						{
							CollisionChecker::FixMoveSP(colA, colB, hitData);
						}
					}
				}
				else if (colKindA == ColKind3D::kCapsule && colKindB == ColKind3D::kSphere)
				{
					hitResult = CollisionChecker::CheckHitCS(colA, colB);
					if (hitResult)
					{
						// �����߂�
						if (!skipPushBack)
						{
							CollisionChecker::FixMoveCS(colA, colB);
						}
					}
				}
				else if (colKindA == ColKind3D::kCapsule && colKindB == ColKind3D::kCapsule)
				{
					hitResult = CollisionChecker::CheckHitCC(colA, colB);
					if (hitResult)
					{
						if (!skipPushBack)
						{
							CollisionChecker::FixMoveCC(colA, colB);
						}
					}
				}
				// ���̓����蔻��𑝂₵�����Ƃ��͂�����else�łȂ���

				if (hitResult)
				{
					actA->OnCollision(actB);
					actB->OnCollision(actA);
				}

				isHit |= hitResult;
			}
		}
		++loopCount;
	}
}

void Physics::DrawColRange(std::list<std::shared_ptr<Actor>> actorList) const
{
	// Collider�̕`��֐����Ă� ���ꂾ��
	for (auto& actor : actorList)
	{
		if (!actor->CanCollide() || !actor->HasCol()) continue;
		actor->GetCol().Draw();
	}
}

void Physics::Gravity(std::list<std::shared_ptr<Actor>> actorList)
{
	for (auto& actor : actorList)
	{
		// �����Ă��Ȃ��\��������
		if (!actor->CanCollide() || !actor->HasRigid()) continue;

		// static�̂�͓������Ȃ�
		if (actor->GetCol().IsStatic()) continue;

		auto& rigid = actor->GetRigid();
		rigid.AddVel(kGravity);
	}
}
