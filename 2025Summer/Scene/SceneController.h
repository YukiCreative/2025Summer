#pragma once
#include <memory>
#include <list>

class Scene;
class ScreenFade;

enum class SceneChangeKind
{
	kStackScene,
	kChangeScene,
	kRemoveScene,
	kResetScene,
	kNone,
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
	// 内部でInitしてる
	void ChangeScene(std::shared_ptr<Scene> nextScene);
	void StackScene(std::shared_ptr<Scene> stackScene);
	void RemoveScene();
	// 今あるシーンを全て消して、新しいシーン一つだけにする
	void ResetScene(std::shared_ptr<Scene> nextScene);

	void StackSceneWithFade(std::shared_ptr<Scene> nextScene);
	void ChangeSceneWithFade(std::shared_ptr<Scene> nextScene);
	void RemoveSceneWithFade();
	void ResetSceneWithFade(std::shared_ptr<Scene> nextScene);

private:

	std::shared_ptr<ScreenFade> m_fade;
	std::shared_ptr<Scene> m_nextScene;
	SceneChangeKind m_changeKind;

	using FadeUpdate_t = void (SceneController::*)();
};