#pragma once
#include <algorithm>

// 範囲が制限された値を作りたいときに使う
// テンプレート型関数がヘッダでしか定義できないとは知らなかった
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