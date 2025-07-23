#include "SceneController.h"
#include "Scene.h"
//#include "SceneTest.h"
#include "SceneDebug.h"
#include <cassert>
#include "ScreenFade.h"

SceneController::SceneController() :
	m_fadeState(FadeState::kFadeIn)
{
}

SceneController& SceneController::GetInstance()
{
	static SceneController instance;
	return instance;
}

void SceneController::Init()
{
	m_fade = std::make_shared<ScreenFade>();
	m_fade->Init();
	StackScene(std::make_shared<SceneDebug>());
}

void SceneController::Update()
{
	m_fade->Update();

	switch (m_fadeState)
	{
	case FadeState::kNormal:
		break;
	case FadeState::kFadeIn:
		// フェードインしきったらnextSceneを遷移してNormalへ
		if (m_fade->IsEndFadeIn())
		{

		}
		break;
	case FadeState::kFadeOut:
		break;
	default:
		assert(false && "FadeStateの値が不正です");
		break;
	}

	m_scenes.back()->Update();
}

void SceneController::Draw() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	m_scenes.back()->Reave();

	// InitとEntry何が違うんだよ
	m_scenes.back() = nextScene;
	m_scenes.back()->Init();
	m_scenes.back()->Entry();
}

void SceneController::StackScene(std::shared_ptr<Scene> stackScene)
{
	if (!m_scenes.empty())
	{
		m_scenes.back()->Reave();
	}

	m_scenes.emplace_back(stackScene);
	m_scenes.back()->Init();
	m_scenes.back()->Entry();
}

void SceneController::RemoveScene()
{
	if (m_scenes.size() <= 1)
	{
		assert(false && "シーンが一つしかないのにRemoveしようとした");
		return;
	}

	m_scenes.back()->Reave();
	m_scenes.pop_back();

	m_scenes.back()->Entry();
}

void SceneController::SingleScene(std::shared_ptr<Scene> nextScene)
{
	if (!m_scenes.empty())
	{
		m_scenes.back()->Reave();
	}
	m_scenes.clear();

	m_scenes.emplace_back(nextScene);
	m_scenes.back()->Init();
	m_scenes.back()->Entry();
}