#include "SceneTest.h"
#include <DxLib.h>
#include "Camera.h"
#include "Input.h"
#include "ActorController.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneController.h"
#include "EnemyTest.h"

namespace
{
	constexpr float kLineOffset = 100;
	constexpr float kLineLength = 500;
}

SceneTest::SceneTest()
{
}

void SceneTest::Init()
{
	m_camera = std::make_shared<Camera>();
	m_camera->Init();

	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	auto player = std::make_shared<Player>();
	player->Init(m_camera, m_actors);
	auto enemy = std::make_shared<EnemyTest>();
	enemy->Init();
	m_actors->AddActor(player);
	m_actors->AddActor(enemy);
}

void SceneTest::Update()
{
	m_camera->Update();
	m_actors->Update();
}

void SceneTest::Draw() const
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

	//m_camera->Draw_Debug();

	m_actors->Draw();

	const Vector2 left = input.GetLeftInputAxis();
	const Vector2 right = input.GetRightInputAxis();
	DrawFormatString(0,0,0xffffff, "左スティック x:%f,y:%f",left.x, left.y);
	DrawFormatString(0,15,0xffffff, "右スティック x:%f,y:%f",right.x, right.y);

	if (input.IsPressed("Right"))
	{
		DrawString(0,30,"右押されてます", 0xffffff);
	}

	DrawGrid(10,10);
}

void SceneTest::Entry()
{
}

void SceneTest::Reave()
{
}

void SceneTest::DrawGrid(const int xNum, const int zNum) const
{
	for (int x = 0; x < xNum; ++x)
	{
		DrawLine3D({ kLineOffset * x - (xNum * 0.5f) * kLineOffset, 0, -kLineLength }, { kLineOffset * x - (xNum * 0.5f) * kLineOffset, 0, kLineLength }, 0xffffff);
	}

	for (int z = 0; z < zNum; ++z)
	{
		DrawLine3D({ -kLineLength, 0, kLineOffset * z - (zNum * 0.5f) * kLineOffset }, { kLineLength, 0, kLineOffset * z - (zNum * 0.5f) * kLineOffset }, 0xffffff);
	}
}
