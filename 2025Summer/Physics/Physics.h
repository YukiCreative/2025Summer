#pragma once
#include <memory>
#include <list>

class Actor;
class Collider3D;

// コライダーを動かし、当たり判定を行い、押し戻しを計算させる
// ActorControllerが持つ
class Physics
{
public:

	// ActorControllerから移動と当たり判定を分離した
	void Update(std::list<std::shared_ptr<Actor>> actorList);

	// 当たり判定の範囲を描画
	// デバッグ用
	void DrawColRange(std::list <std::shared_ptr<Actor>>) const;
private:
	// 重力を処理
	void Gravity(std::list<std::shared_ptr<Actor>> actorList);
	// 当たり判定
	void CheckHit(std::list<std::shared_ptr<Actor>>& actorList);
	// Exitの判定も行っている
	void SendOnCollision();
private:

	struct OnCollisionMessage
	{
		std::shared_ptr<Actor> hitActor;
		std::shared_ptr<Actor> other;

		bool operator ==(const OnCollisionMessage& right) const
		{
			return (hitActor == right.hitActor && other == right.other);
		}
	};


	std::list<OnCollisionMessage> m_beforeCollisionMessageList;
	std::list<OnCollisionMessage> m_enterMessageList;
	std::list<OnCollisionMessage> m_stayMessageList;

	// 重複をはじく
	// この衝突がEnter,Stayなのかも判定
	void AddOnCollisionMessage(const OnCollisionMessage& message);
};