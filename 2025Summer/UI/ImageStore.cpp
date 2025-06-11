#include "ImageStore.h"
#include <DxLib.h>
#include <filesystem>
#include <vector>
#include <cassert>

namespace
{
	const std::string kFolderPath = "Data/Image/";

	const std::vector<std::string> kImageFileExtensions =
	{
		".png",
		".jpg",
		".bmp",
	};
}

ImageStore::ImageStore()
{
	LoadImages();
}

ImageStore::~ImageStore()
{
	// 画像の解放
	// 全部やっちゃえ
	InitGraph();
}

ImageStore& ImageStore::GetInstance()
{
	static ImageStore instance;
	return instance;
}

int ImageStore::GetGraph(const std::string fileName)
{
	assert(m_handles.contains(fileName) && "ファイル名の画像がありません");
	return m_handles.at(fileName);
}

void ImageStore::LoadImages()
{
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(kFolderPath))
	{
		const auto& path = file.path();

		// 画像として読める拡張子かどうかを確かめてる
		bool isValidExtention = std::find(kImageFileExtensions.begin(),
			kImageFileExtensions.end(), path.extension().string()) != kImageFileExtensions.end();
		if (!isValidExtention)
		{
#if _DEBUG
			printf("拡張子が違うファイルをスキップ\n");
			printf("　パス：%s\n", path.string().c_str());
#endif
			continue;
		}

		// 読み込み
		std::string pathCopy = path.string();
		const std::string fileName = pathCopy.erase(0, kFolderPath.size());
		const int handle  = LoadGraph(path.string().c_str());
		m_handles[fileName] = handle;

#if _DEBUG
		assert(handle != -1 && "読み込みに失敗した");
		printf("画像ファイルを読み込みました\n");
		printf("　ファイル名：%s\n", fileName.c_str());
		printf("　ハンドル：%d\n", handle);
#endif
	}
}
