#pragma once
#include "Actor.h"

class AnimationModel;
class Camera;
class PlayerState;


// アクセスレベルがpublicになっているのは、ステートクラスに情報を明け渡すため
// 悪用厳禁！
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	// 外部から見てもいい関数

	Player();

	void Init(const std::weak_ptr<Camera> camera);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

public:
	// 自分かPlayerStateだけで見たいメンバ変数
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerState> m_state;

public:
	// 自分かPlayerStateだけで見てほしい関数
	void Move(const float moveSpeed);
	void CameraMove();
};

