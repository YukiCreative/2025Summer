#pragma once
#include <string>

class Music
{
private:
	Music();
	Music(const Music&) = delete;
	void operator=(const Music&) = delete;

	int m_playType;

	std::string m_nowPlayingFilePath;
public:
	static Music& GetInstance();
	void Init();
	void Play(const std::string& path);
	void Loop(const bool& loopOrNot);
	bool IsPlaying() const;
	void Stop() const;
};