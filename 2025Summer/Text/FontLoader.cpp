#include "FontLoader.h"
#include <DxLib.h>
#include <filesystem>

namespace
{
	const std::string kFontFolder = "Data/Font/";
}

FontLoader::FontLoader()
{
	// ここで、特定のフォルダのファイルを全ロード
	// フォルダなのかディレクトリなのかどっちかにしてほしい
	std::filesystem::directory_iterator directory = std::filesystem::directory_iterator(kFontFolder);
	for (const auto& file : directory)
	{
		const std::string path = file.path().string();
		AddFontResourceExA(path.c_str(), FR_PRIVATE, NULL);
#if _DEBUG
		printf("フォントを読み込みました\n");
		printf("パス：%s\n", path.c_str());
#endif
	}
}

void FontLoader::LoadFontFile()
{
	static FontLoader instance;
}

FontLoader::~FontLoader()
{
	RemoveFontResourceExA("", FR_PRIVATE, NULL);
}