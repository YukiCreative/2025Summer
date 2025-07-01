#pragma once

class Application
{
public:
	static Application& GetInstance();

	bool Init();
	void Run();
	void Tarminate() const;

	void QuitGame();
private:
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	// true�Ȃ���s�X�L�b�v
	// false�Œʂ�
	bool DebugPause();

private:

	bool m_isRunning;
	bool m_isPause;
};

