#include "Timer.h"
#include "../General/Game.h"

Timer::Timer() :
	m_count(0),
	m_state(&Timer::CountDownUpdate),
	m_isStop(true),
	m_measureTime(0)
{
}

void Timer::Init()
{
}

bool Timer::Update()
{
	return (this->*m_state)();
}

bool Timer::CountDownUpdate()
{
	if (m_isStop) return false;

	// カウントダウン
	--m_count;

	if (m_count < 0) return true;

	++m_measureTime;

	return false;
}

bool Timer::CountUpUpdate()
{
	if (m_isStop) return false;

	// カウントアップ
	++m_count;

	if (m_count < 0) return true;

	++m_measureTime;

	return false;
}

const int Timer::GetCountFrame() const
{
	return m_count;
}

const float Timer::GetCountSecond() const
{
	return static_cast<float>(m_count) / static_cast<float>(Game::kFrameRate);
}

void Timer::StartCount()
{
	m_isStop = false;
}

void Timer::StopCount()
{
	m_isStop = true;
}

void Timer::SetStateCountUp()
{
	m_state = &Timer::CountUpUpdate;
}

void Timer::SetStateCountDown()
{
	m_state = &Timer::CountDownUpdate;
}

void Timer::StartMeasurement()
{
	m_measureTime = 0;
}

float Timer::GetMeasureSecond() const
{
	return static_cast<float>(m_measureTime) / static_cast<float>(Game::kFrameRate);
}
