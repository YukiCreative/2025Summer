#include "UIPositionStore.h"
#include <DxLib.h>
#include <string>

namespace
{
	const std::string kFilePath = "Data/UIPositionData/UIPositionData.dat";
}

UIPositionStore::UIPositionStore()
{
	LoadPosition();
}

UIPositionStore& UIPositionStore::GetInstance()
{
	static UIPositionStore instance;
	return instance;
}

void UIPositionStore::LoadPosition()
{
	const int fileH = FileRead_open(kFilePath.c_str());

	for (int i = 0; i < static_cast<int>(UIKind::kMax); ++i)
	{	
		FileRead_read(&(m_dataMap[static_cast<UIKind>(i)]), sizeof(UIPositionData), fileH);
	}
}

UIPositionData& UIPositionStore::GetPositionData(const UIKind& kind)
{
	return m_dataMap[kind];
}
