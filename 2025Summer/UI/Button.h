#pragma once
#include "Vector2.h"
#include <string>
#include <memory>

// 役割
// カーソルがフォーカスできる
// フォーカス時に押せる
// 押すと設定されたイベントが実行される
// イベントはButtonを継承したクラスで実装
// ボタンのフォーカスやカーソルの処理は別のクラスでやる
class Button abstract : public std::enable_shared_from_this<Button>
{
public:
	Button();

	void Init(const Vector2& initPos);
	// Updateは処理を書いているので上書きしない
	void Update();
	virtual void Draw() const abstract;

	// フォーカスされたらButtonSystemが呼ぶ関数
	virtual void OnFocused() abstract;
	virtual void OnDisfocused() abstract;
	// ここに押下時の個別処理を
	virtual void OnPressed() abstract;

	void SetRightButton(std::weak_ptr<Button> right) { m_rightButton = right; }
	void SetLeftButton(std::weak_ptr<Button> left) { m_leftButton = left; }
	void SetUpButton(std::weak_ptr<Button> up) { m_upButton = up; }
	void SetDownButton(std::weak_ptr<Button> down) { m_downButton = down; }

	std::weak_ptr<Button> RightButton() const;
	std::weak_ptr<Button> LeftButton()  const;
	std::weak_ptr<Button> UpButton()    const;
	std::weak_ptr<Button> DownButton()  const;

protected:
	Vector2 m_pos;

	// フォーカスの有無の切り替え
	void InvertState();
	virtual void FocusedUpdate() abstract;
	virtual void NormalUpdate() abstract;

private:
	// 上下左右の入力がされた時のボタンの遷移先
	std::weak_ptr<Button> m_leftButton;
	std::weak_ptr<Button> m_rightButton;
	std::weak_ptr<Button> m_upButton;
	std::weak_ptr<Button> m_downButton;
	// 画像とかテキストとかは派生先で宣言した方が扱いやすいと思って置いていない

	using UpdateState_t = void (Button::*)();
	UpdateState_t m_updateState;
};