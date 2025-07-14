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

	// ƒ‰ƒ“ƒ_ƒ€ƒGƒ“ƒWƒ“‚Æ‚©‚¢‚¤‚Ì‚ğ•Ô‚µ‚Ü‚·
	// ‚±‚ê‚ğ“ü‚ê‚Ä—”‚ğì‚é—””­¶Ší‚ª‚ ‚é‚Ì‚Å
	std::default_random_engine& GetRandomEngine() { return m_randomEngine; }

private:
	std::default_random_engine m_randomEngine;
};