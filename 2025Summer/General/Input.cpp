#include "Input.h"
#include <cassert>
#include <DxLib.h>

namespace
{
	constexpr int kPadA      = PAD_INPUT_1;
	constexpr int kPadB      = PAD_INPUT_2;
	constexpr int kPadX      = PAD_INPUT_3;
	constexpr int kPadY      = PAD_INPUT_4;
	constexpr int kPadL      = PAD_INPUT_5;
	constexpr int kPadR      = PAD_INPUT_6;
	constexpr int kPadSelect = PAD_INPUT_7;
	constexpr int kPadStart  = PAD_INPUT_8;
	constexpr int kPadLStick = PAD_INPUT_9;
	constexpr int kPadRStick = PAD_INPUT_10;
	constexpr int kPadUp     = PAD_INPUT_UP;
	constexpr int kPadDown   = PAD_INPUT_DOWN;
	constexpr int kPadRight  = PAD_INPUT_RIGHT;
	constexpr int kPadLeft   = PAD_INPUT_LEFT;
}

Input::Input() :
	m_padInput(0),
	m_beforePadInput(0),
	m_keyInput(),
	m_beforeKeyInput(),
	m_leftInputAxis(),
	m_beforeLeftInputAxis(),
	m_inputEvent()
{
}

Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}

void Input::SetKeyDefault()
{
	m_inputEvent["Submit"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RETURN},
		{PeripheralType::kPad, kPadA},
	};
	m_inputEvent["Up"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_W},
		{PeripheralType::kKeyboard, KEY_INPUT_UP},
		{PeripheralType::kPad, PAD_INPUT_UP},
	};
	m_inputEvent["Down"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_S},
		{PeripheralType::kKeyboard, KEY_INPUT_DOWN},
		{PeripheralType::kPad, PAD_INPUT_DOWN},
	};
	m_inputEvent["Right"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_D},
		{PeripheralType::kKeyboard, KEY_INPUT_RIGHT},
		{PeripheralType::kPad, PAD_INPUT_RIGHT},
	};
	m_inputEvent["Left"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_A},
		{PeripheralType::kKeyboard, KEY_INPUT_LEFT},
		{PeripheralType::kPad, PAD_INPUT_LEFT},
	};
	m_inputEvent["Rise"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RSHIFT}
	};
	m_inputEvent["Descend"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RCONTROL}
	};

	m_inputEvent["GoDebug"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_F1}
	};
	m_inputEvent["Jump"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_SPACE},
		{PeripheralType::kPad, kPadA}
	};
	m_inputEvent["CameraUp"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_I}
	};
	m_inputEvent["CameraLeft"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_J}
	};
	m_inputEvent["CameraDown"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_K}
	};
	m_inputEvent["CameraRight"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_L}
	};
	m_inputEvent["LockOn"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_LSHIFT},
		{PeripheralType::kPad, kPadR},
	};
	m_inputEvent["Pause"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_P},
		{PeripheralType::kPad, kPadStart},
	};
	m_inputEvent["FrameForward"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_AT},
		{PeripheralType::kPad, kPadSelect},
	};
	m_inputEvent["Attack"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_Z},
		{PeripheralType::kPad, kPadX},
	};
	m_inputEvent["SpecialAttack"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_X},
		{PeripheralType::kPad, kPadY},
	};
}

void Input::Init()
{
	SetKeyDefault();
}

void Input::Update()
{
	// PADの前のフレームのを記憶
	m_beforePadInput = m_padInput;
	m_padInput = GetJoypadInputState(DX_INPUT_PAD1);
	// キー
	m_beforeKeyInput = m_keyInput;
	GetHitKeyStateAll(m_keyInput.data());

	// スティックの入力を取得
	int dPadX = 0, dPadY = 0;
	// キーボード、十字キーも取得
	if (IsPressed("Up"))	dPadY += -1000;
	if (IsPressed("Down"))  dPadY += 1000;
	if (IsPressed("Right")) dPadX += 1000;
	if (IsPressed("Left"))  dPadX += -1000;
	int lStickX = 0, lStickY = 0;
	GetJoypadAnalogInput(&lStickX, &lStickY, DX_INPUT_PAD1);
	// 十字キーとアナログスティックの入力を両立するための処理
	if (lStickX == 0)	lStickX = dPadX;
	if (lStickY == 0)	lStickY = dPadY;

	// 右スティックをキーボードで代替する入力を取得
	int rKeyX = 0, rKeyY = 0;
	if (IsPressed("CameraUp"))	  rKeyY += -1000;
	if (IsPressed("CameraDown"))  rKeyY += 1000;
	if (IsPressed("CameraRight")) rKeyX += 1000;
	if (IsPressed("CameraLeft"))  rKeyX += -1000;

	int rightX, rightY;
	GetJoypadAnalogInputRight(&rightX, &rightY, DX_INPUT_PAD1);
	if (rightX == 0)	rightX = rKeyX;
	if (rightY == 0)	rightY = rKeyY;

	// アナログ入力の大きさを1000までに制限する
	Vector2 rightInputAxis(static_cast<float>(rightX), static_cast<float>(rightY));
	if (rightInputAxis.SqrMagnitude() > 1000 * 1000)
	{
		rightInputAxis.Normalize();
		rightInputAxis *= 1000;
	}
	Vector2 leftInputAxis(static_cast<float>(lStickX), static_cast<float>(lStickY));
	if (leftInputAxis.SqrMagnitude() > 1000 * 1000)
	{
		leftInputAxis.Normalize();
		leftInputAxis *= 1000;
	}

	// ここで前フレームの入力を覚えておく
	m_beforeLeftInputAxis = m_leftInputAxis;
	m_beforeRightInputAxis = m_rightInputAxis;
	// そして更新
	m_leftInputAxis = leftInputAxis;
	m_rightInputAxis = rightInputAxis;
}

bool Input::IsPressed(const std::string& key) const
{
	bool isPressed = false;
	// 与えられたキーに対応する全ての周辺機器の入力コードを調べる
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
			// orで取っているのは仕様です
		case PeripheralType::kKeyboard:
			// キーボードのやり方で入力を取得
			isPressed = isPressed || m_keyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// パッド
			isPressed = isPressed || m_padInput & inputEvent.inputCode;
			break;
		default:
			assert(false && "列挙体の要素に対して処理が実装されていない");
			break;
		}
	}
	return isPressed;
}

bool Input::IsTrigger(const std::string& key) const
{
	bool isTrigger = false;
	// 与えられたキーに対応する全ての周辺機器の入力コードを調べる
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
		case PeripheralType::kKeyboard:
			// キーボードのやり方で入力を取得
			isTrigger = isTrigger || m_keyInput[inputEvent.inputCode] && !m_beforeKeyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// パッド
			isTrigger = isTrigger || (m_padInput & inputEvent.inputCode) && !(m_beforePadInput & inputEvent.inputCode);
			break;
		default:
			assert(false && "列挙体の要素に対して処理が実装されていない");
			break;
		}
	}
	return isTrigger;
}

bool Input::IsReleased(const std::string& key) const
{
	bool isReleased = false;
	// 与えられたキーに対応する全ての周辺機器の入力コードを調べる
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		// 今押されていなくて、前のフレームに押されていた場合にtrue
		switch (inputEvent.type)
		{
		case PeripheralType::kKeyboard:
			// キーボードのやり方で入力を取得
			isReleased = isReleased || !m_keyInput[inputEvent.inputCode] && m_beforeKeyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// パッド
			isReleased = isReleased || !(m_padInput & inputEvent.inputCode) && (m_beforePadInput & inputEvent.inputCode);
			break;
		default:
			assert(false && "列挙体の要素に対して処理が実装されていない");
			break;
		}
	}
	return isReleased;
}

const Vector2& Input::GetLeftInputAxis() const
{
	return m_leftInputAxis;
}

const Vector2& Input::GetBeforeLeftInputAxis() const
{
	return m_beforeLeftInputAxis;
}

const Vector2& Input::GetRightInputAxis() const
{
	return m_rightInputAxis;
}

const Vector2& Input::GetBeforeRightInputAxis() const
{
	return m_beforeRightInputAxis;
}
