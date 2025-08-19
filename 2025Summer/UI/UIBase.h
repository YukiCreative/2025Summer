#pragma once
#include <memory>
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector2.h"
#include "UIKind.h"
#include <string>

#define ToString(type) #type

class Image;

// UI‚ÌŠî’êƒNƒ‰ƒX
// 3D‚Ì•`‰æŒã‚É•`‰æ‚³‚ê‚é
class UIBase abstract
{
public:
	UIBase(const std::string& className);

	virtual void Update() abstract;
	virtual void Draw() const abstract;

	const std::string& GetName() const { return m_name; }
	void SetPos(const Vector2& pos) { m_pos = pos; }
	const Vector2& GetPos() const {return m_pos; }

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() const { return m_scale; }

protected:

	Vector2 m_pos;
	float m_scale;
	std::string m_name;
};

