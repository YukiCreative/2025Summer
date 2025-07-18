#include "SceneController.h"
#include "Scene.h"
//#include "SceneTest.h"
#include "SceneDebug.h"
#include <cassert>

SceneController::SceneController()
{
}

SceneController& SceneController::GetInstance()
{
	static SceneController instance;
	return instance;
}

void SceneController::Init()
{
	StackScene(std::make_shared<SceneDebug>());
}

void SceneController::Update()
{
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