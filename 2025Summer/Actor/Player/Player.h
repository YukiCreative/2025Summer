#pragma once
#include "Actor.h"
#include <list>
#include "PlayerIntermediateState.h"
#include "RangeLimitedValue.h"

class AnimationModel;
class Camera;
class ActorController;
class Image;
class PlayerSword;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

enum class PlayerInputDir
{
	kFront,
	kRight,
	kLeft,
	kBack
};

namespace
{
	constexpr float kMinHP = 0.0f;
	constexpr float kMaxHP = 3000.0f;
}

using PlayerHP = RangeLimitedValue<float, kMinHP, kMaxHP>;

// アクセスレベルがpublicになっているのは、ステートクラスに情報を明け渡すため
// 悪用厳禁！
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	// 外部から見てもいい関数

	Player();
	~Player();

	void Init(const std::weak_ptr<Camera> camera, std::weak_ptr<ActorController> cont);

	void Update() override;
	void Draw() const override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// 使ってるモデルの向きが特殊なのでそれ用に修正した行列を返す
	DxLib::tagMATRIX GetModelMatrix() const;

	Vector3 GetRightInexPos() const;
	// 自分の右手の小指のフレーム→人差し指のフレームのベクトル(単位)
	Vector3 GetRightHandVec() const;

	// 剣自体の見た目を表示する
	void EnableSword();
	// 剣の当たり判定を有効化する
	void EnableSwordCol(const float attackPower, const float knockbackPower = 0.0f);

	void DisableSword();
	void DisableSwordCol();
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsInvincible() const { return m_isInvincible; }

	Vector3 GetLockOnActorScreenPos() const;
	bool IsLockOn() const { return !m_lockOnActor.expired(); }
	bool CanLockOn() const { return m_canLockOn; }
	float GetHpRatio() const;

public:
	// 自分かPlayerStateだけで見たいメンバ変数
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerIntermediateState> m_state;
	Vector3 m_targetPos;
	std::weak_ptr<Actor> m_lockOnActor;
	std::weak_ptr<ActorController> m_cont;
	std::shared_ptr<PlayerSword> m_sword;
	bool m_isContactLockOnActor;
	// プレイヤーの移動方向の履歴
	// コマンドに使用
	std::list<PlayerInputDir> m_inputList;
	PlayerHP m_hp;
	bool m_isInvincible;
	bool m_canLockOn;

public:
	// 自分かPlayerStateだけで見てほしい関数
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	void SetInputDir(const PlayerInputDir& dir);
	// PlayerShockWaveSlash用
	void SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback);
	void OnDamage(std::shared_ptr<AttackCol> attack);
	// プレイヤーをロックオン対象へ向ける
	void LockOnRotate();
	void ReleaseLockOn();
	void SetCanLockOn(const bool canOrNot) { m_canLockOn = canOrNot; }
};