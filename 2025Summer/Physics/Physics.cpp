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

	SendOnCollision();
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
			// �L���łȂ��R���C�_�[�͖���
			if (!actA->GetCol().IsValid()) continue;
			
			for (auto& actB : actorList)
			{
				if (!actB->GetCol().IsValid()) continue;

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
					AddOnCollisionMessage({actA, actB});
					AddOnCollisionMessage({actB, actA});
				}

				isHit |= hitResult;
			}
		}
		++loopCount;
	}
}

void Physics::SendOnCollision()
{
	for (auto& enterMessage : m_enterMessageList)
	{
		enterMessage.hitActor->OnCollisionEnter(enterMessage.other);
	}

	for (auto& stayMessage : m_stayMessageList)
	{
		stayMessage.hitActor->OnCollisionStay(stayMessage.other);
	}

	// �O�ɓ������Ă��āA���������Ă��Ȃ���exit
	for (auto& beforeMessage : )
	{
		bool isExit = false;
		for (auto& enterMessage : m_enterMessageList)
		{
			isExit |= (beforeMessage == enterMessage);
		}
		for (auto& stayMessage : m_stayMessageList)
		{
			isExit |= (beforeMessage == stayMessage);
		}
		if (isExit)
		{
			beforeMessage.hitActor->OnCollisionExit(beforeMessage.other);
		}
	}

	std::sort(m_enterMessageList.begin(), m_enterMessageList.end());
	std::sort(m_stayMessageList.begin(), m_stayMessageList.end());
	m_beforeCollisionMessageList = std::merge(m_enterMessageList.begin(), m_enterMessageList.end(),
		m_stayMessageList.begin(), m_stayMessageList.end());

	m_enterMessageList.clear();
	m_stayMessageList.clear();
}

void Physics::AddOnCollisionMessage(const OnCollisionMessage& message)
{
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

		// �E�`�d�͂���Ȃ����
		if (!actor->GetCollidable().GetRigid().IsUseGravity()) continue;

		auto& rigid = actor->GetRigid();
		rigid.AddVel(kGravity);
	}
}
