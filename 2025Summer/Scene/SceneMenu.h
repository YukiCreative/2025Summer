#pragma once
#include "Scene.h"
#include <memory>
#include "../UI/Button.h"

class ButtonSystem;
class Image;

class SceneMenu : public Scene
{
public:
	SceneMenu();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

private:
	enum class TitleTransitionSceneKind
	{
		kTutorial,
		kCampaign,
		kArena,
		kRanking,
		kOption,
		kTitle,
		kNone,
	};

	std::shared_ptr<ButtonSystem> m_buttons;

	class MenuButton : public Button
	{
	public:
		MenuButton();

		void Init(const Vector2 initPos, const TitleTransitionSceneKind kind);

		virtual void Draw() const override;

		// フォーカスされたらButtonSystemが呼ぶ関数
		virtual void OnFocused() override;
		virtual void OnDisfocused() override;
		// ここに押下時の個別処理を
		virtual void OnPressed() override;

	private:

		TitleTransitionSceneKind m_kind;
		std::shared_ptr<Image> m_image;

	private:

		virtual void FocusedUpdate() override;
		virtual void NormalUpdate() override;
	};
};

