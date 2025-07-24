#include "ButtonSystem.h"
#include "Input.h"
#include "Button.h"
#include "ButtonNoFocus.h"
#include <cassert>

namespace
{
	constexpr int kButtonPosMoveThreshold = 300;
	constexpr int kCursorMoveInterval = 20;
	constexpr int kAxisDirThreshold = 45;

	const std::string kSerectSound = "ButtonSelectSound.mp3";
}

ButtonSystem::ButtonSystem() :
	m_cursorMoveTimer(0),
	m_inputDirLastMove(0,0)
{
	m_noFocus = std::make_shared<ButtonNoFocus>();
	m_nowFocusedButton = m_noFocus;
}

ButtonSystem::~ButtonSystem()
{
}

void ButtonSystem::Init()
{

}

void ButtonSystem::Update()
{
	--m_cursorMoveTimer;
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();

	// 選択ボタンの移動処理
	MoveFocus(inputAxis);

	// 登録されているボタンの更新処理をする
	for (auto& button : m_buttons)
	{
		button->Update();
	}

	// ボタン押したときの処理
	if (Input::GetInstance().IsTrigger("Submit"))
	{
		m_nowFocusedButton.lock()->OnPressed();
		// すぐにシーン遷移するかもしれないのでreturnしとく
		return;
	}
}

void ButtonSystem::Draw() const
{
	// ボタンの描画関数を実行
	for (const auto& button : m_buttons)
	{
		button->Draw();
		//printf("参照数:%d", button.use_count());
	}
}

void ButtonSystem::AddButton(std::shared_ptr<Button> buttonInstance)
{
	m_buttons.push_back(buttonInstance);
}

void ButtonSystem::SetButtonFocus(std::shared_ptr<Button> setButton)
{
	assert(std::find(m_buttons.begin(), m_buttons.end(), setButton) != m_buttons.end() && "登録されていないボタンです");
	m_nowFocusedButton.lock()->OnDisfocused();
	m_nowFocusedButton.lock()->InvertState();
	m_nowFocusedButton = setButton;
	m_nowFocusedButton.lock()->OnFocused();
	m_nowFocusedButton.lock()->InvertState();
}

void ButtonSystem::ExitFocus()
{
	m_nowFocusedButton = std::static_pointer_cast<Button>(m_noFocus);
}

void ButtonSystem::MoveFocus(Vector2 inputAxis)
{
	if (!CanMoveFocus(inputAxis))
	{
		//printf("動けなかった\n");
		return;
	}

	Vector2 inputDir(0, 0);
	std::weak_ptr<Button> beforeButton = m_nowFocusedButton;
	// axisを取ってフォーカスの位置をずらす
	if (inputAxis.x > kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->RightButton();
		++inputDir.x;
	}
	else if (inputAxis.x < -kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->LeftButton();
		--inputDir.x;
	}
	if (inputAxis.y > kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->DownButton();
		++inputDir.y;
	}
	else if (inputAxis.y < -kButtonPosMoveThreshold)
	{
		m_nowFocusedButton = m_nowFocusedButton.lock()->UpButton();
		--inputDir.y;
	}

	// ここで何も変わっていなかったら処理終わり
	if (m_nowFocusedButton.lock() == beforeButton.lock()) return;

	// 今の入力を記憶
	m_inputDirLastMove = inputDir;

	// 新しく選択されたボタンに「お前選ばれてるぞ」と連絡
	m_nowFocusedButton.lock()->OnFocused();
	m_nowFocusedButton.lock()->InvertState();
	// 元のボタンは通常状態に戻るように連絡
	beforeButton.lock()->OnDisfocused();
	beforeButton.lock()->InvertState();
	// タイマーを設定
	m_cursorMoveTimer = kCursorMoveInterval;
	// ついでに音
	//SoundManager::GetInstance().Play(kSerectSound);
}

bool ButtonSystem::CanMoveFocus(const Vector2& inputAxis) const
{
	// 条件
	// 　前の有効な入力方向と同じ方向（入力なし以外）に入力していたら、
	//　 その方向に入力を初めて一定時間が経過していないといけない
	// 前のフレームと方向が違うならそのまま入力を通す
	// 入力方向は360度を上下左右で分割した四つ

	Vector2 beforeInputAxis = Input::GetInstance().GetBeforeLeftInputAxis();

	// 前回入力していない時はTrueでいいよ
	if (beforeInputAxis.x == 0 && beforeInputAxis.y == 0) return true;

	if (abs(inputAxis.RerativeAngle(m_inputDirLastMove)) > 30) return true;

	if (m_cursorMoveTimer <= 0) return true;

	return false;
}