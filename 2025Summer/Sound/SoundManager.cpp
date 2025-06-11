#include "SoundManager.h"
#include "Sound.h"
#include <filesystem>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string kFolderPath = "Data/Sound/";

	const std::vector<std::string> kSoundFileExtensions =
	{
		".mp3",
		".wav",
	};
}

SoundManager::SoundManager()
{
}

void SoundManager::SetAllSoundVolume(int volume)
{
	for (auto& sound : m_sounds)
	{
		// こっちでバリデーションチェックしてくれるから大丈夫
		sound->SetVolume(volume);
	}
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::Init()
{
	LoadSourceSound();
}

void SoundManager::Update()
{
	RemoveFinishedSounds();
}

void SoundManager::RemoveFinishedSounds()
{
	SoundList_t playingSounds;
	for (const auto& sound : m_sounds)
	{
		if (sound->IsPlaying())
		{
			playingSounds.push_back(sound);
		}
	}
	// 再生中のだけ残して後は参照しない→解放
	m_sounds = playingSounds;

	//printf("今の音源インスタンス：%d個\n", m_sounds.size());
}

std::weak_ptr<Sound> SoundManager::Play(const std::string& fileName, const bool isLoop)
{
	auto sound = std::make_shared<Sound>();
	sound->Init(m_store[fileName], isLoop);
	m_sounds.emplace_back(sound);
	return sound;
}

void SoundManager::LoadSourceSound()
{
	// 指定したフォルダ内にある音声ファイルを全読み込み
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(kFolderPath))
	{
		std::string path = file.path().string();

		// 画像として読める拡張子かどうかを確かめてる
		bool isValidExtention = std::find(kSoundFileExtensions.begin(),
			kSoundFileExtensions.end(), file.path().extension().string()) != kSoundFileExtensions.end();
		if (!isValidExtention)
		{
#if _DEBUG
			printf("拡張子が違うファイルをスキップ\n");
			printf("　パス：%s\n", path.c_str());
#endif
			continue;
		}

		// ファイル名を取り出す
		std::string pathCopy = path;
		std::string fileName = pathCopy.erase(0, kFolderPath.size());
		m_store[fileName] = LoadSoundMem(path.c_str());

#if _DEBUG
		assert(m_store[fileName] != -1 && "音声ファイル読み込みに失敗した");
		printf("音声ファイルを読み込みました。\n");
		printf("　ファイル名：%s\n", fileName.c_str());
		printf("　ハンドル：%d\n", m_store[fileName]);
#endif
	}
}
