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

	// ランダムエンジンとかいうのを返します
	// これを入れて乱数を作る乱数発生器があるので
	std::default_random_engine& GetRandomEngine() { return m_randomEngine; }

private:
	std::default_random_engine m_randomEngine;
};