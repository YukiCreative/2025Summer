#include "SceneTest.h"
#include <DxLib.h>
#include "../Camera/Camera.h"
#include "../General/Input.h"
#include "../Actor/ActorController.h"
#include "../UI/UIController.h"
#include "../Actor/Player/Player.h"
#include "SceneDebug.h"
#include "SceneController.h"
#include "../Shader/ShadowMap.h"
#include "../UI/PlayerLockOnUI.h"
#include "../UI/PlayerSpecialGauge.h"
#include "../UI/PlayerHPBar.h"
#include "../GameManagement/ArenaMode.h"
#include "../GameManagement/Score/StylishRank.h"
#include "../UI/StylishRankUI.h"
#include "../Model/Model.h"
#include "../Geometry/Geometry.h"
#include "../Shader/SkyBox.h"
#include "../Sound/Music.h"
#include "SceneTitle.h"

namespace
{
	constexpr float kLineOffset = 100;
	constexpr float kLineLength = 500;

	const std::string kFieldModel = "Data/Model/Field.mv1";

	const Vector3& kShadowAreaMin = { -1500, -1000, -1500 };
	const Vector3& kShadowAreaMax = {  1500,  1000,  1500 };

	const std::string kSkyBoxImg = "SkyBox_Dark.png";

	const std::string kBGMName = "Data/BGM/BattleBGM.wav";
}

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	m_field = std::make_shared<Model>();
	m_field->Init(kFieldModel);
	m_field->SetPos({ 0,-100, 0 });

	m_camera = std::make_shared<Camera>();
	m_camera->Init();

	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	auto player = std::make_shared<Player>();
	player->Init(m_camera, m_actors);
	m_actors->AddActor(player);

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
	auto stylishUI = std::make_shared<StylishRankUI>();
	stylishUI->Init();
	m_UI->AddUI(stylishUI);

	m_gameManager = std::make_shared<ArenaMode>();
	m_gameManager->Init(player, m_actors, m_UI);

	m_skyBox = std::make_shared<SkyBox>();
	m_skyBox->Init(kSkyBoxImg, m_camera);

	Music::GetInstance().Play(kBGMName);

	Input::GetInstance().AddEvent("GoTitle", {PeripheralType::kKeyboard, KEY_INPUT_ESCAPE});
}

void SceneTest::Update()
{
	auto& input = Input::GetInstance();

	m_camera->Update();
	if (!m_UI->IsEditing())
	{
		m_actors->Update();
	}
	m_UI->Update();
	m_skyBox->Update();

	m_gameManager->Update();

	StylishRank::GetInstance().Update();

#if _DEBUG
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneDebug>());
		return;
	}
#endif

	if (input.IsTrigger("GoTitle"))
	{
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTitle>());
	}
}

void SceneTest::Draw() const
{
	m_skyBox->Draw();

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

	DrawFormatString(300, 0, 0xffffff, "FPS:%.2f", GetFPS());

	DrawFormatString(500, 0, 0xffffff, "StylishPoint:%f", StylishRank::GetInstance().GetStylishPoint());
	DrawFormatString(700, 0, 0xffffff, "StylishRank:%d", StylishRank::GetInstance().GetRankUIParam().m_rank);
	DrawFormatString(900, 0, 0xffffff, "StylishRankRatio:%f", StylishRank::GetInstance().GetRankUIParam().m_ratio.Value());
#endif
}

void SceneTest::Entry()
{
}

void SceneTest::Reave()
{
}