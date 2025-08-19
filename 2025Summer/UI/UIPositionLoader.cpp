#include "UIPositionLoader.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

namespace
{
	const std::string kFilePath = "Data/UIPositionData/UIPositionData.csv";
	const std::string kHeader = "Name,PosX,PosY,Scale";
};

UIPositionLoader::UIPositionLoader()
{
}

UIPositionLoader& UIPositionLoader::GeInstance()
{
	static UIPositionLoader instance;
	return instance;
}

void UIPositionLoader::LoadPosition()
{
	// csvを読み込み
#if _DEBUG
	printf("UI座標の読み込み開始\n");
#endif 

	std::ifstream input(kFilePath);

	// ヘッダーを確認
	std::string header;
	std::getline(input, header);
	if (header != kHeader)
	{
		assert(false && "ファイルのヘッダが違う");
	}

	std::string strBuf;

	while (std::getline(input, strBuf))
	{
		std::istringstream iStream(strBuf);

		// 名前、位置、スケールの順で入ってくる
		std::string name, posX, posY, sScale;
		std::getline(iStream, name, ',');
		std::getline(iStream, posX, ',');
		std::getline(iStream, posY, ',');
		std::getline(iStream, sScale, ',');

		const Vector2 pos = { std::stof(posX), std::stof(posY) };
		const float scale = std::stof(sScale);

		m_dataMap[name] = {pos, scale};
#if _DEBUG
		printf("名前  :%s", name.c_str());
		printf("X:%f,Y:%f", pos.x, pos.y);
		printf("拡大率:%f", scale);
#endif
	}

	input.close();

#if _DEBUG
	printf("UI座標の読み込み完了\n");
#endif 
}

void UIPositionLoader::SavePosition()
{
#if _DEBUG
	// csvを書き出し
	printf("書き出します\n");

	std::ofstream output(kFilePath);

	output << kHeader << std::endl;

	for (auto& data : m_dataMap)
	{
		output << data.first << ','
			<< data.second.pos.x << ','
			<< data.second.pos.y << ','
			<< data.second.scale << std::endl;
	}

	output.close();

	printf("書き出し完了");

#endif
}

const UIPosData& UIPositionLoader::GetPosData(const std::string& name)
{
	return m_dataMap[name];
}

void UIPositionLoader::SetPosData(const std::string& name, const Vector2& pos, const float scale)
{
	m_dataMap[name] = UIPosData(pos, scale);
}