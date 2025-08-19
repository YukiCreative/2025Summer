#pragma once
#include <unordered_map>
#include "UIKind.h"
#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include "UIBase.h"
#include <string>

struct UIPosData
{
public:
	Vector2 pos;
	float scale;
};

// UIの位置を読み込んでいつでも出せるようにしておきます
class UIPositionLoader
{
private:
	UIPositionLoader();
	UIPositionLoader(const UIPositionLoader&) = delete;
	void operator=(const UIPositionLoader&) = delete;

public:
	static UIPositionLoader& GeInstance();

	// 外部ファイルから座標とスケールを読み込みます
	void LoadPosition();
	void SavePosition();
	const UIPosData& GetPosData(const std::string& name);

	void SetPosData(const std::string& name, const Vector2& pos, const float scale);

private:

	std::unordered_map<std::string, UIPosData> m_dataMap;
};

