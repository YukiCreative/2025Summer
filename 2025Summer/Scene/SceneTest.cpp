#include "SceneTest.h"
#include <DxLib.h>
#include "Camera.h"
#include "Input.h"
#include "ActorController.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneController.h"

#include "Model.h"
#include "Geometry.h"
#include "EnemyGenerator.h"

namespace
{
	constexpr float kLineOffset = 100;
	constexpr float kLineLength = 500;

	const std::string kFieldModel = "Data/Model/Field.mv1";
}

SceneTest::SceneTest() :
	m_dirH(-1)
{
}

SceneTest::~SceneTest()
{
	DeleteLightHandle(m_dirH);
}

void SceneTest::Init()
{
	m_field = std::make_shared<Model>();
	m_field->Init(kFieldModel);
	m_field->SetPos({0,-100, 0});

	//m_dirH = CreateDirLightHandle(Vector3::Up());

	m_camera = std::make_shared<Camera>();
	m_camera->Init();

	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	auto player = std::make_shared<Player>();
	player->Init(m_camera, m_actors);
	auto enemyGenerator = std::make_shared<EnemyGenerator>();
	enemyGenerator->Init(player);
	enemyGenerator->SpawnWave(0);
	m_actors->AddActor(player);
	m_actors->AddActor(enemyGenerator);
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

	m_field->Draw();

	m_actors->Draw();

	//DrawGrid(10,10);
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
