#pragma once
#include <list>
#include <memory>
#include "Vector2.h"
#include "UIBase.h"

class Button;
class ButtonNoFocus;

// 役割
// ボタンをたくさん持つ
// 入力をボタンに拘束、解除
// どのボタンが選ばれているか教えてあげる
// ボタンが押されたとき、そのボタンに教えてあげる　
class ButtonSystem : public UIBase
{
public:
	ButtonSystem();
	~ButtonSystem();

	void Init();

	// 入力からどのボタンを選択しているかを変えたりする
	void Update() override;
	
	void Draw() const override;

	// ボタンを追加
	// シーンが利用する想定
	void AddButton(std::shared_ptr<Button> buttonInstance);

	void SetButtonFocus(std::shared_ptr<Button> setButton);
	void ExitFocus();

private:

	using ButtonList_t = std::list<std::shared_ptr<Button>>;
	ButtonList_t m_buttons;
	std::weak_ptr<Button> m_nowFocusedButton;
	// 非フォーカス時を表すボタンを作る
	std::shared_ptr<ButtonNoFocus> m_noFocus;
	// 同じ方向に入力し続けたら動く仕様のタイマー
	int m_cursorMoveTimer;
	Vector2 m_inputDirLastMove;

	void MoveFocus(Vector2 inputAxis);

	bool CanMoveFocus(const Vector2& inputAxis) const;
};