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
		{PeripheralType::kKeyboard, KEY_INPUT_UP},
		{PeripheralType::kPad, PAD_INPUT_UP},
	};
	m_inputEvent["Down"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_DOWN},
		{PeripheralType::kPad, PAD_INPUT_DOWN},
	};
	m_inputEvent["Right"] =
	{
		{PeripheralType::kKeyboard, KEY_INPUT_RIGHT},
		{PeripheralType::kPad, PAD_INPUT_RIGHT},
	};
	m_inputEvent["Left"] =
	{
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
}

void Input::Init()
{
	SetKeyDefault();
}

void Input::Update()
{
	// PAD�̑O�̃t���[���̂��L��
	m_beforePadInput = m_padInput;
	m_padInput = GetJoypadInputState(DX_INPUT_PAD1);
	// �L�[
	m_beforeKeyInput = m_keyInput;
	GetHitKeyStateAll(m_keyInput.data());

	// �X�e�B�b�N�̓��͂��擾
	int dPadX = 0, dPadY = 0;
	// �L�[�{�[�h�A�\���L�[���擾
	if (IsPressed("Up"))	dPadY += -1000;
	if (IsPressed("Down"))  dPadY += 1000;
	if (IsPressed("Right")) dPadX += 1000;
	if (IsPressed("Left"))  dPadX += -1000;
	int lStickX = 0, lStickY = 0;
	GetJoypadAnalogInput(&lStickX, &lStickY, DX_INPUT_PAD1);
	// �\���L�[�ƃA�i���O�X�e�B�b�N�̓��͂𗼗����邽�߂̏���
	if (lStickX == 0)	lStickX = dPadX;
	if (lStickY == 0)	lStickY = dPadY;

	int rightX, rightY;
	GetJoypadAnalogInputRight(&rightX, &rightY, DX_INPUT_PAD1);

	// �A�i���O���͂̑傫����1000�܂łɐ�������
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

	// �����őO�t���[���̓��͂��o���Ă���
	m_beforeLeftInputAxis = m_leftInputAxis;
	m_beforeRightInputAxis = m_rightInputAxis;
	// �����čX�V
	m_leftInputAxis = leftInputAxis;
	m_rightInputAxis = rightInputAxis;
}

bool Input::IsPressed(const std::string& key) const
{
	bool isPressed = false;
	// �^����ꂽ�L�[�ɑΉ�����S�Ă̎��Ӌ@��̓��̓R�[�h�𒲂ׂ�
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
			// or�Ŏ���Ă���͎̂d�l�ł�
		case PeripheralType::kKeyboard:
			// �L�[�{�[�h�̂����œ��͂��擾
			isPressed = isPressed || m_keyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// �p�b�h
			isPressed = isPressed || m_padInput & inputEvent.inputCode;
			break;
		default:
			assert(false && "�񋓑̗̂v�f�ɑ΂��ď�������������Ă��Ȃ�");
			break;
		}
	}
	return isPressed;
}

bool Input::IsTrigger(const std::string& key) const
{
	bool isTrigger = false;
	// �^����ꂽ�L�[�ɑΉ�����S�Ă̎��Ӌ@��̓��̓R�[�h�𒲂ׂ�
	for (const auto& inputEvent : m_inputEvent.at(key))
	{
		switch (inputEvent.type)
		{
		case PeripheralType::kKeyboard:
			// �L�[�{�[�h�̂����œ��͂��擾
			isTrigger = isTrigger || m_keyInput[inputEvent.inputCode] && !m_beforeKeyInput[inputEvent.inputCode];
			break;
		case PeripheralType::kPad:
			// �p�b�h
			isTrigger = isTrigger || (m_padInput & inputEvent.inputCode) && !(m_beforePadInput & inputEvent.inputCode);
			break;
		default:
			assert(false && "�񋓑̗̂v�f�ɑ΂��ď�������������Ă��Ȃ�");
			break;
		}
	}
	return isTrigger;
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
