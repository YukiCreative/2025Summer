#include "Sound.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	constexpr int kVolumeMin = 0;
	constexpr int kVolumeMax = 255;
}

Sound::Sound() :
	m_handle(-1),
	m_playType(DX_PLAYTYPE_BACK)
{
}

Sound::~Sound()
{
	DeleteSoundMem(m_handle);
}

void Sound::Init(const int handle, const bool isLoop)
{
	m_handle = DuplicateSoundMem(handle);
	Loop(isLoop);
	Play();
}

void Sound::Loop(const bool isLoop)
{
	m_playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
}

void Sound::Play() const
{
	PlaySoundMem(m_handle, m_playType);
}

void Sound::Stop() const
{
	StopSoundMem(m_handle);
}

bool Sound::IsPlaying() const
{
	return CheckSoundMem(m_handle);
}

void Sound::SetVolume(const int param) const
{
	int vol = std::clamp(param, kVolumeMin, kVolumeMax);
	ChangeVolumeSoundMem(param, m_handle);
}