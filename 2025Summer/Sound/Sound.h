#pragma once
#include <string>

class SoundManager;

// ‰¹
class Sound
{
private:
	int m_handle;
	int m_playType;
public:
	Sound();
	~Sound();

	void Init(const int handle, const bool isLoop = false);

	void Play() const;
	void Stop() const;
	bool IsPlaying() const;
	void SetVolume(const int param) const;

private:
	void Loop(const bool value);
};