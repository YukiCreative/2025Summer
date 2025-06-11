#pragma once

class Application
{
public:
	static Application& GetInstance();

	bool Init();
	void Run() const;
	void Tarminate() const;

	void QuitGame();
private:
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	bool m_isRunning;
};

