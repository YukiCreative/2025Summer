#pragma once

// 時間が測れます
class Timer
{
public:
	Timer();

	void Init();
	// タイマーが0を下回るとtrueが返ってきます
	bool Update();

	const int    GetCountFrame() const;
	const float GetCountSecond() const;

	void StartCount();
	void StopCount();
	bool IsStop() const { return m_isStop; }
	void SetCount(const int count) { m_count = count; }
	void SetStateCountUp();
	void SetStateCountDown();
	void AddCount(const int count) { m_count += count; }

private:
	using State_t = bool (Timer::*)();
	bool CountDownUpdate();
	bool CountUpUpdate  ();

private:
	// このカウントはフレーム数
	int m_count;
	State_t m_state;
	bool m_isStop;
};

