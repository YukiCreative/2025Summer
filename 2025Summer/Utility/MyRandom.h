#pragma once
#include <random>

class MyRandom
{
private:
	MyRandom();
	MyRandom(const MyRandom&) = delete;
	void operator = (const MyRandom&) = delete;
public: 
	static MyRandom& GetInstance();

	void Init();

	// �����_���G���W���Ƃ������̂�Ԃ��܂�
	// ��������ė�������闐�������킪����̂�
	std::default_random_engine& GetRandomEngine() { return m_randomEngine; }

private:
	std::default_random_engine m_randomEngine;
};