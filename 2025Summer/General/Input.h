#pragma once
#include "Vector2.h"
#include <array>
#include <vector>
#include <string>
#include <unordered_map>

class Input
{
public:
	static Input& GetInstance();

	// キーコンフィグを初期値に
	void SetKeyDefault();

	void Init();
	void Update();

	// 押されている
	bool IsPressed(const std::string& key) const;
	// 今まさに押された
	bool IsTrigger(const std::string& key) const;
	// 今まさに離された
	bool IsReleased(const std::string& key) const;

	// アナログコントローラ＆十字キーの入力
	const Vector2& GetLeftInputAxis() const;
	const Vector2& GetBeforeLeftInputAxis() const;
	const Vector2& GetRightInputAxis() const;
	const Vector2& GetBeforeRightInputAxis() const;

private:
	enum class PeripheralType
	{
		kKeyboard,
		kPad,
	};

	struct Peripheral
	{
		PeripheralType type;
		int inputCode;
	};

private:
	// いつものごとくシングルトン
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	int m_padInput;
	int m_beforePadInput;

	std::array<char, 256> m_keyInput;
	std::array<char, 256> m_beforeKeyInput;

	Vector2 m_leftInputAxis;
	Vector2 m_beforeLeftInputAxis;
	Vector2 m_rightInputAxis;
	Vector2 m_beforeRightInputAxis;

	std::unordered_map<std::string, std::vector<Peripheral>> m_inputEvent;
};

