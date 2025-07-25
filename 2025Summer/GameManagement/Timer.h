#pragma once

// ���Ԃ�����܂�
class Timer
{
public:
	Timer();

	void Init();
	// �^�C�}�[��0��������true���Ԃ��Ă��܂�
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
	// ���̃J�E���g�̓t���[����
	int m_count;
	State_t m_state;
	bool m_isStop;
};

