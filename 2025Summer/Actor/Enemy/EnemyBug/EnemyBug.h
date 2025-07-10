#pragma once
#include "Enemy.h"

class AnimationModel;
class EnemyBugState;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

class EnemyBug : public Enemy, public std::enable_shared_from_this<EnemyBug>
{
public:
	EnemyBug();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;
	void OnCollisionStay(std::shared_ptr<Actor> other) override;
	void OnCollisionExit(std::shared_ptr<Actor> other) override;

	// ステートにも変数を隠してみるか
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	DxLib::tagMATRIX GetModelMatrix() const;
	void AddVel(const Vector3& vel);
	bool IsAnimEnd() const;

private:

	std::shared_ptr<EnemyBugState> m_state;

private:

	void OnDamage(std::shared_ptr<AttackCol> other);
};

