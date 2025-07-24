#include "Scene.h"
#include "SceneController.h"
#include "SceneDebug.h"
#include "ScreenFade.h"
#include <cassert>

SceneController::SceneController() :
	m_changeKind(SceneChangeKind::kStackScene)
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
	StackSceneWithFade(std::make_shared<SceneDebug>());
}

void SceneController::Update()
{
	if (m_fade->IsEndFadeOut())
	{
		switch (m_changeKind)
		{
		case SceneChangeKind::kStackScene:
			StackScene(m_nextScene);
			break;
		case SceneChangeKind::kChangeScene:
			ChangeScene(m_nextScene);
			break;
		case SceneChangeKind::kRemoveScene:
			RemoveScene();
			break;
		case SceneChangeKind::kResetScene:
			ResetScene(m_nextScene);
			break;
		default:
			assert(false && "SceneChangeKindの値が不正です。");
			break;
		}

		m_changeKind = SceneChangeKind::kNone;
		m_nextScene = nullptr;
		m_fade->FadeIn();
	}

	m_fade->Update();

	if (m_scenes.empty()) return;

	m_scenes.back()->Update();
}

void SceneController::Draw() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Draw();
	}

	m_fade->Draw();
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

void SceneController::ResetScene(std::shared_ptr<Scene> nextScene)
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

void SceneController::StackSceneWithFade(std::shared_ptr<Scene> nextScene)
{
	m_fade->FadeOut();
	m_nextScene = nextScene;
	m_changeKind = SceneChangeKind::kStackScene;
}

void SceneController::ChangeSceneWithFade(std::shared_ptr<Scene> nextScene)
{
	m_fade->FadeOut();
	m_nextScene = nextScene;
	m_changeKind = SceneChangeKind::kChangeScene;
}

void SceneController::RemoveSceneWithFade()
{
	m_fade->FadeOut();
	m_changeKind = SceneChangeKind::kRemoveScene;
}

void SceneController::ResetSceneWithFade(std::shared_ptr<Scene> nextScene)
{
	m_fade->FadeOut();
	m_nextScene = nextScene;
	m_changeKind = SceneChangeKind::kResetScene;
}
