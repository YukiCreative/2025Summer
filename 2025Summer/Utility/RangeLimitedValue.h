#pragma once
#include <algorithm>

// �͈͂��������ꂽ�l����肽���Ƃ��Ɏg��
// �e���v���[�g�^�֐����w�b�_�ł�����`�ł��Ȃ��Ƃ͒m��Ȃ�����
template <typename T, T Min, T Max>
struct RangeLimitedValue
{
public:
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
	RangeLimitedValue<T, Min, Max> operator+(const RangeLimitedValue& other)
	{
		return m_value + other.Value();
	}
	void operator-=(const RangeLimitedValue& other)
	{
		*this = *this - other;
	}
	RangeLimitedValue<T, Min, Max> operator-(const RangeLimitedValue& other)
	{
		return m_value - other.Value();
	}
};