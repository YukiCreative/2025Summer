#pragma once
#include <list>
#include <string>
#include <memory>
#include <unordered_map>

class Sound;

class SoundManager
{
private:
	SoundManager();
	SoundManager(const SoundManager&) = delete;
	void operator=(const SoundManager&) = delete;
public:
	static SoundManager& GetInstance();

	void Init();

	// 毎フレーム呼び出す
	void Update();

	// UnityのInstantiateみたいな感じで、生成した後にその参照を返す
	std::weak_ptr<Sound> Play(const std::string& fileName, const bool isLoop = false);
	// 一括で変えられる
	// 0(無音)~255(デフォルト)
	void SetAllSoundVolume(int volume);

private:
	using SoundList_t = std::list<std::shared_ptr<Sound>>;
	SoundList_t m_sounds;

	std::unordered_map<std::string, int> m_store;

private:
	void RemoveFinishedSounds();
	void LoadSourceSound();
};

