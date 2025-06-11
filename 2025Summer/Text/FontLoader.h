#pragma once
#include <string>
#include <unordered_map>

// フォントファイルをwindowsに一時的に取り込むだけ
// シングルトンが増えてきた
class FontLoader
{
private:
	FontLoader();
	FontLoader(const FontLoader&) = delete;
	void operator=(const FontLoader&) = delete;

	using FontMap_t = std::unordered_map<std::string, int>;
	FontMap_t m_fontHandles;
public:
	// 何回実行しても一度しか実行されない
	static void LoadFontFile();

	~FontLoader();
};

