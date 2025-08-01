#pragma once
#include <string>
#include "../Geometry/Vector2.h"
#include "../Geometry/Geometry.h"
#include "../Utility/RangeLimitedValue.h"

using BlendParam = RangeLimitedValue<float, 0.0f, 255.0f>;

class Image
{
public:
	Image();

	void Init(const int& handle);
	void Init(const std::string& fileName);

	void Draw(const Vector2& pos) const;
	// �ݒ肵���g��l���K�p����Ȃ��̂Œ���
	void RectDraw(const Vector2& pos, const Vector2& rectStart, const Vector2& rectEnd) const;
	void RectRotaDraw(const Vector2& pos, const Vector2& rectStart, const Vector2& rectEnd, const float rad = 0.0f) const;

	void SetGraph(const std::string& fileName);

	// ���݂̉��{�̑傫���ɂ��邩
	void ExpandGraph(const float& exRate);
	// ���
	void SetExRate(const float& value)    { m_expandRate = value; }
	// ���󂩂牽�x��]�����邩
	void RotateGraph(const float& deg)    { m_angle += deg * Geometry::kDegToRad; }
	// ���
	void SetAngle(const float& angleDeg)  { m_angle = angleDeg * Geometry::kDegToRad; }
	// SetDrawBlendMode�����̂܂ܕ\�Ɍ����Ă���
	void SetImageBlendMode(const int& blendmode, const float& pal)
	{
		m_blendMode = blendmode;
		m_blendPal = pal;
	}
	void SetImageBlendMode(const int& blendMode) { m_blendMode = blendMode; }
	void SetImageBlendParam (const float& pal)       { m_blendPal = pal; }
	void SetReverceFlagX  (const bool& value)	 { m_reverceX = value; }
	void SetReverceFlagY  (const bool& value)	 { m_reverceY = value; }
	void SetOffset		  (const Vector2& value) { m_offset = value; }
	int GraphHandle() const { return m_sourceHandle; }
	
	const float& ExRate() const { return m_expandRate; }
	const float GetBlendParam() const { return m_blendPal.Value(); }
	const Vector2 GetImageSize() const;

	// �蓮���
	void DeleteImage();

private:
	int     m_sourceHandle;
	float   m_expandRate;
	float   m_angle;
	int     m_blendMode;
	BlendParam m_blendPal;
	bool    m_reverceX;
	bool    m_reverceY;
	Vector2 m_offset;
};

