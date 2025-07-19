#include "Physics.h"
#include "Actor.h"
#include "Collider3D.h"
#include "SphereCollider.h"
#include "PolygonCollider.h"
#include "CollisionChecker.h"
#include "Collidable.h"
#include "Rigid.h"
#include <algorithm>
#include <iostream>

namespace
{
	// 重力は定数
	// 後で変数にするかも？？？
	const Vector3 kGravity = { 0, -0.8f, 0 };

	constexpr int kCheckLoopMax = 1;
}

void Physics::Update(std::list<std::shared_ptr<Actor>> actorList)
{
	// 重力
	Gravity(actorList);

	CheckHit(actorList);

	SendOnCollision();
}

void Physics::CheckHit(std::list<std::shared_ptr<Actor>>& actorList)
{
	bool isHit = true;
	int loopCount = 0;

	// 当たらなくなるか、基底の回数ループするまで処理を継続
	while (isHit && loopCount < kCheckLoopMax)
	{
		isHit = false;

		for (auto& actA : actorList)
		{
			// 有効でないコライダーは無視
			if (!actA->GetCol().IsValid()) continue;
			
			for (auto& actB : actorList)
			{
				if (!actB->GetCol().IsValid()) continue;

				if (!actA->CanCollide() || !actB->CanCollide()) continue;

				// 同一人物なら計算しない
				if (actA == actB) continue;

				Collidable& colA = actA->GetCollidable();
				Collidable& colB = actB->GetCollidable();

				// どちらも今フレームに動いていなければ当たっていない
				if (colA.IsStop() && colB.IsStop()) continue;

				const ColKind3D colKindA = colA.GetColKind();
				const ColKind3D colKindB = colB.GetColKind();

				// すり抜けるかどうか
				const bool skipPushBack = colA.IsThrough() || colB.IsThrough();
				bool hitResult = false;

				// 二つのColliderの種類に応じた当たり判定関数を呼ぶ
				if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kSphere)
				{
					hitResult = CollisionChecker::CheckHitSS(colA, colB);

					if (hitResult)
					{
						// 押し戻しする
						// 透過設定を確認して、どちらも不透過なら押し戻し
						if (!skipPushBack)
						{
							// 押し戻し
							CollisionChecker::FixMoveSS(colA, colB);
						}
					}
				}
				else if (colKindA == ColKind3D::kSphere && colKindB == ColKind3D::kPolygon)
				{
					PolyHitData hitData;
					// これ反対のケースも列挙しないといけないのゴミコード過ぎん？
					hitResult = CollisionChecker::CheckHitSP(colA, colB, hitData);

					if (hitResult)
					{
						// 押し戻し
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
						// 押し戻し
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
				// 他の当たり判定を増やしたいときはここにelseでつなげる

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

	// 前に当たっていて、今当たっていない→exit
	for (auto& beforeMessage : m_beforeCollisionMessageList)
	{
		bool isExit = true;
		for (auto& enterMessage : m_enterMessageList)
		{
			if (beforeMessage == enterMessage)
			{
				isExit = false;
				break;
			}
		}
		for (auto& stayMessage : m_stayMessageList)
		{
			if (beforeMessage == stayMessage)
			{
				isExit = false;
				break;
			}
		}
		if (isExit)
		{
			beforeMessage.hitActor->OnCollisionExit(beforeMessage.other);
		}
	}

	m_beforeCollisionMessageList.clear();

	// enterとstayをまとめてbeforeに入れる
	// その方が扱いやすい
	std::copy(m_enterMessageList.begin(), m_enterMessageList.end(), std::back_inserter(m_stayMessageList));
	std::copy(m_stayMessageList.begin(), m_stayMessageList.end(), std::back_inserter(m_beforeCollisionMessageList));

	m_enterMessageList.clear();
	m_stayMessageList.clear();
}

void Physics::AddOnCollisionMessage(const OnCollisionMessage& message)
{
	bool isBeforeSend = false;

	// 前のフレームに送られていたらstay
	for (auto& beforeMessage : m_beforeCollisionMessageList)
	{
		isBeforeSend |= (beforeMessage == message);
	}

	if (isBeforeSend)
	{
		// の前にすでに送られているかチェック
		for (auto& stay : m_stayMessageList)
		{
			if (stay == message) return;
		}

		m_stayMessageList.emplace_back(message);
	}
	else
	{
		// すでに送られているかチェック
		for (auto& enter : m_enterMessageList)
		{
			if (enter == message) return;
		}

		m_enterMessageList.emplace_back(message);
	}
}

void Physics::DrawColRange(std::list<std::shared_ptr<Actor>> actorList) const
{
	// Colliderの描画関数を呼ぶ それだけ
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
		// 持っていない可能性がある
		if (!actor->CanCollide() || !actor->HasRigid()) continue;

		// staticのやつは動かさない
		if (actor->GetCol().IsStatic()) continue;

		// ウチ重力いらないんで
		if (!actor->GetCollidable().GetRigid().IsUseGravity()) continue;

		auto& rigid = actor->GetRigid();
		rigid.AddVel(kGravity);
	}
}
