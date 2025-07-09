#pragma once
#include "Actor.h"
#include <list>
#include "PlayerState.h"
#include "RangeLimitedValue.h"

class AnimationModel;
class Camera;
class PlayerState;
class ActorController;
class Image;
class PlayerSword;

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
	constexpr float kMaxHP = 99990.0f;
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

	void OnCollision(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// 使ってるモデルの向きが特殊なのでそれ用に修正した行列を返す
	DxLib::tagMATRIX GetModelMatrix() const;

	Vector3 GetRightInexPos() const;
	// 自分の右手の小指のフレーム→人差し指のフレームのベクトル(単位)
	Vector3 GetRightHandVec() const;

	// 剣自体の見た目を表示する
	void EnableSword();
	// 剣の当たり判定を有効化する
	void EnableSwordCol(const float attackPower);

	void DisableSword();
	void DisableSwordCol();

public:
	// 自分かPlayerStateだけで見たいメンバ変数
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerState> m_state;
	Vector3 m_targetPos;
	std::weak_ptr<Actor> m_lockOnActor;
	std::weak_ptr<ActorController> m_cont;
	Vector3 m_lockOnCursorPos;
	std::shared_ptr<Image> m_lockOnGraph;
	std::shared_ptr<PlayerSword> m_sword;
	bool m_isContactLockOnActor;
	// プレイヤーの移動方向の履歴
	// コマンドに使用
	std::list<PlayerInputDir> m_inputList;
	PlayerHP m_hp;

public:
	// 自分かPlayerStateだけで見てほしい関数
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	float DefaultGroundDrag();
	float DefaultAirDrag();
	void SetInputDir(const PlayerInputDir& dir);
	// PlayerShockWaveSlash用
	void SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk);
	void OnDamage(const float damage);
	// プレイヤーをロックオン対象へ向ける
	void LockOnRotate();
};