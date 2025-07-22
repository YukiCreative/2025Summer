#pragma once
#include <memory>
#include "Vector3.h"
#include "Vector2.h"

class Image;

// UIの基底クラス
// 3Dの描画後に描画される
class UIBase abstract
{
public:
	UIBase();

	virtual void Update() abstract;
	virtual void Draw() const abstract;

protected:

	// Zによって重なる順番を操作できてもいいかも
	// 7/16現在はそんな機能はない
	Vector3 m_pos;
};

