#pragma once
#include <algorithm>

// �͈͂��������ꂽ�l����肽���Ƃ��Ɏg��
// �e���v���[�g�^�֐����w�b�_�ł�����`�ł��Ȃ��Ƃ͒m��Ȃ�����
// �����R�Ȃ���Z�p���Z�q����������Ă��Ȃ��^�ł͎g���܂���
template <typename T, T Min, T Max>
struct RangeLimitedValue
{
private:
	const T kMinValue = Min;
	const T kMaxValue = Max;

	T m_value;

public:
	RangeLimitedValue()
	{
		m_value = kMinValue;
	}
	RangeLimitedValue(const T value)
	{
		m_value = std::clamp(value, kMinValue, kMaxValue);
	}
	void operator=(const RangeLimitedValue& other)
	{
		m_value = other.Value();
	}

	const T Value() const { return m_value; }

	void operator+=(const RangeLimitedValue& other)
	{
		*this = *this + other;
	}
	void operator+=(const T other)
	{
		*this = *this + other;
	}
	RangeLimitedValue<T, Min, Max> operator+(const RangeLimitedValue& other)
	{
		return m_value + other.Value();
	}
	RangeLimitedValue<T, Min, Max> operator+(const T other)
	{
		return m_value + other;
	}
	void operator-=(const RangeLimitedValue& other)
	{
		*this = *this - other;
	}
	void operator-=(const T other)
	{
		*this = *this - other;
	}
	RangeLimitedValue<T, Min, Max> operator-(const RangeLimitedValue& other)
	{
		return m_value - other.Value();
	}
	RangeLimitedValue<T, Min, Max> operator-(const T other)
	{
		return m_value - other;
	}

	// �ő吔���ǂ���
	bool IsMax() const { return m_value == kMaxValue; }
	// �ŏ������ǂ���
	bool IsMin() const { return m_value == kMinValue; }
	void SetMax() { m_value = kMaxValue; }
	void SetMin() { m_value = kMinValue; }	
	// ���̒l���ő�l�ɂ������Ăǂꂭ�炢�̊����Ȃ̂��Ԃ�
	// 0�`1
	// float�ɃL���X�g�ł���^�ł��邱�ƁB
	float GetRatio() const
	{
		float ratio = static_cast<float>(m_value) / static_cast<float>(kMaxValue);
		return std::clamp(ratio, 0.0f, 1.0f);
	}

	// �^����ꂽ�l���ő�l�̂ǂꂮ�炢�̊������Ԃ��܂�
	float GetRatio(const T value)
	{
		float ratio = static_cast<float>(value) / static_cast<float>(kMaxValue);
		return std::clamp(ratio, 0.0f, 1.0f);
	}
};