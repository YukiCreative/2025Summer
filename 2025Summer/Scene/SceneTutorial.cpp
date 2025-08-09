#include "SceneTutorial.h"
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

namespace
{
	constexpr float kLineOffset = 100;
	constexpr float kLineLength = 500;

	const std::string kFieldModel = "Data/Model/Field.mv1";

	const Vector3& kShadowAreaMin = { -1500, -300, -1500 };
	const Vector3& kShadowAreaMax = { 1500,  300,  1500 };
}

SceneTutorial::SceneTutorial()
{
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
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
}

void SceneTutorial::Update()
{
	m_actors->Update();
	m_camera->Update();
	m_UI->Update();

	if (Input::GetInstance().IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneDebug>());
		return;
	}
}

void SceneTutorial::Draw() const
{
	m_shadow->StartShadowMapDraw(GetLightDirection());

	m_actors->Draw();

	m_shadow->EndShadowMapDraw();

	m_actors->Draw();
	m_field->Draw();
	m_UI->Draw();

	m_shadow->UnsetShadowMap();
}

void SceneTutorial::Entry()
{
}

void SceneTutorial::Reave()
{
}
