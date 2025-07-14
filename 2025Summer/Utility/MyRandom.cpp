#include "MyRandom.h"

MyRandom::MyRandom()
{
}

MyRandom& MyRandom::GetInstance()
{
	static MyRandom instance;
	return instance;
}

void MyRandom::Init()
{
	std::random_device randomDevice;
	m_randomEngine = std::default_random_engine(randomDevice());
}