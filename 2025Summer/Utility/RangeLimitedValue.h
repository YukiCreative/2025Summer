#pragma once
#include <algorithm>

// 範囲が制限された値を作りたいときに使う
// テンプレート型関数がヘッダでしか定義できないとは知らなかった
// ※当然ながら算術演算子が実装されていない型では使えません
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

	// 最大数かどうか
	bool IsMax() const { return m_value == kMaxValue; }
	// 最小数かどうか
	bool IsMin() const { return m_value == kMinValue; }
	void SetMax() { m_value = kMaxValue; }
	void SetMin() { m_value = kMinValue; }	
	// 今の値が最大値にたいしてどれくらいの割合なのか返す
	// 0～1
	// floatにキャストできる型であること。
	float GetRatio() const
	{
		float ratio = static_cast<float>(m_value) / static_cast<float>(kMaxValue);
		return std::clamp(ratio, 0.0f, 1.0f);
	}

	// 与えられた値が最大値のどれぐらいの割合か返します
	float GetRatio(const T value)
	{
		float ratio = static_cast<float>(value) / static_cast<float>(kMaxValue);
		return std::clamp(ratio, 0.0f, 1.0f);
	}
};