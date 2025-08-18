#pragma once
#include <memory>
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector2.h"
#include "UIKind.h"

class Image;

struct UIPositionData
{
public:
	Vector2 pos;
	Vector3 scale;
};

// UI‚ÌŠî’êƒNƒ‰ƒX
// 3D‚Ì•`‰æŒã‚É•`‰æ‚³‚ê‚é
class UIBase abstract
{
public:
	UIBase(const UIKind& m_kind);

	virtual void Update() abstract;
	virtual void Draw() const abstract;

protected:

	UIPositionData m_positionData;
	UIKind m_kind;
};

