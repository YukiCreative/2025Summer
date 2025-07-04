#pragma once
#include "Actor.h"

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

	void DiaableSword();
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

public:
	// 自分かPlayerStateだけで見てほしい関数
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	float DefaultGroundDrag();
	float DefaultAirDrag();
	void LockOn();
};

