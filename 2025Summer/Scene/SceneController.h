#pragma once
#include <memory>
#include <list>

class Scene;
class ScreenFade;

enum class FadeState
{
	kNormal,
	kFadeIn,
	kFadeOut
};

class SceneController
{
private:
	SceneController();
	SceneController(const SceneController&) = delete;
	void operator=(const SceneController&) = delete;

	using SceneList_t = std::list<std::shared_ptr<Scene>>;
	SceneList_t m_scenes;
public:
	static SceneController& GetInstance();

	void Init();
	void Update();
	void Draw() const;
	// ������Init���Ă�
	void ChangeScene(std::shared_ptr<Scene> nextScene);
	void StackScene(std::shared_ptr<Scene> stackScene);
	void RemoveScene();
	// ������V�[����S�ď����āA�V�����V�[��������ɂ���
	void SingleScene(std::shared_ptr<Scene> nextScene);

private:

	std::shared_ptr<ScreenFade> m_fade;
	std::shared_ptr<Scene> m_nextScene;
	FadeState m_fadeState;
};