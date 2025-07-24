#pragma once
#include "Scene.h"
#include <memory>
#include "Button.h"

class ButtonSystem;
class ActorController;
class UIController;
class ShadowMap;
class Camera;
class Image;

class SceneTitle : public Scene
{
public:

	SceneTitle();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();

private:

	std::shared_ptr<ActorController> m_actors;
	std::shared_ptr<UIController> m_UI;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<ShadowMap> m_shadow;

private:

	// スタートボタン
	class StartButton : public Button
	{
	public:
		StartButton();

		void Init(const Vector2& initPos);

		void Draw() const override;

		virtual void OnFocused() override;
		virtual void OnDisfocused() override;

		virtual void OnPressed() override;

	private:

		virtual void FocusedUpdate() override;
		virtual void NormalUpdate() override;

		std::shared_ptr<Image> m_image;
		bool m_isAnim;
		int m_frame;
	};

};