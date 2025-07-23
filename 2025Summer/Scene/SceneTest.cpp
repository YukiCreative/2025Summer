#include "SceneTest.h"
#include <DxLib.h>
#include "Camera.h"
#include "Input.h"
#include "ActorController.h"
#include "UIController.h"
#include "Player.h"
#include "SceneDebug.h"
#include "SceneController.h"
#include "ShadowMap.h"
#include "PlayerLockOnUI.h"
#include "PlayerHPBar.h"
#include "PlayerSpecialGauge.h"

#include "Model.h"
#include "Geometry.h"
#include "EnemyGenerator.h"

namespace
{
	constexpr float kLineOffset = 100;
	constexpr float kLineLength = 500;

	const std::string kFieldModel = "Data/Model/Field.mv1";

	const Vector3& kShadowAreaMin = { -1500, -300, -1500 };
	const Vector3& kShadowAreaMax = {  1500,  300,  1500 };
}

SceneTest::SceneTest() :
	m_dirH(-1),
	m_wave(0)
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

	m_camera = std::make_shared<Camera>();
	m_camera->Init();

	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	auto player = std::make_shared<Player>();
	player->Init(m_camera, m_actors);
	m_enemyGenerator = std::make_shared<EnemyGenerator>();
	m_enemyGenerator->Init(player);
	m_actors->AddActor(player);
	m_actors->AddActor(m_enemyGenerator);

	m_shadow = std::make_shared<ShadowMap>();
	m_shadow->Init(kShadowAreaMin, kShadowAreaMax);

	m_UI = std::make_shared<UIController>();
	m_UI->Init();
	
	// UI‚Æ‚©
	auto lockOn = std::make_shared<PlayerLockOnUI>();
	lockOn->Init(player);
	m_UI->AddUI(lockOn);
	auto hpBar = std::make_shared<PlayerHPBar>();
	hpBar->Init(player);
	m_UI->AddUI(hpBar);
	auto specialBar = std::make_shared<PlayerSpecialGauge>();
	specialBar->Init(player);
	m_UI->AddUI(specialBar);
}

void SceneTest::Update()
{
	m_camera->Update();
	m_actors->Update();
	m_UI->Update();

	m_wave = 0;
	// 
	if (m_actors->SearchEnemy().size() == 0)
	{
		//if (m_wave == 2)
		//{
		//	SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		//}

		m_enemyGenerator->SpawnWave(m_wave);
		++m_wave;
	}
}

void SceneTest::Draw() const
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

	DrawFormatString(300, 0, 0xffffff, "FPS:%.2f", GetFPS());

	m_shadow->StartShadowMapDraw(GetLightDirection());

	m_field->Draw();
	m_actors->Draw();

	m_shadow->EndShadowMapDraw();

	m_field->Draw();
	m_actors->Draw();

	m_shadow->UnsetShadowMap();

	// 3D‚Ì•`‰æ‚ªI‚í‚Á‚½Œã‚ÉUI‚ð•`‰æ
	m_UI->Draw();

#if _DEBUG
	DrawLine3D({0,100,0}, {100, 100, 0}, 0xff0000);
	DrawLine3D({0,100,0}, {0, 200, 0}, 0x00ff00);
	DrawLine3D({0,100,0}, {0, 100, 100}, 0x0000ff);
#endif
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
