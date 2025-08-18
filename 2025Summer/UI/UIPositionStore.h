#pragma once
#include <unordered_map>
#include "UIKind.h"
#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include "UIBase.h"

// UIの位置を読み込んでいつでも出せるようにしておきます
class UIPositionStore
{
private:
	UIPositionStore();
	UIPositionStore(const UIPositionStore&) = delete;
	void operator=(const UIPositionStore&) = delete;

public:
	static UIPositionStore& GetInstance();

	// 外部ファイルから座標とスケールを読み込みます
	void LoadPosition();
	// 指定された種類のUIの座標データを取得します
	UIPositionData& GetPositionData(const UIKind& kind);

private:

	std::unordered_map<UIKind, UIPositionData> m_dataMap;
};

