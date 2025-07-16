#pragma once
#include <string>
#include "Vector2.h"
#include "Geometry.h"
#include "RangeLimitedValue.h"

using BlendParam = RangeLimitedValue<float, 0.0f, 255.0f>;

class Image
{
public:
	Image();

	void Init(const int& handle);
	void Init(const std::string& fileName);

	void Draw(const Vector2& pos) const;

	void SetGraph(const std::string& fileName);

	// åªç›ÇÃâΩî{ÇÃëÂÇ´Ç≥Ç…Ç∑ÇÈÇ©
	void ExpandGraph(const float& exRate);
	// ë„ì¸
	void SetExRate(const float& value)    { m_expandRate = value; }
	// åªèÛÇ©ÇÁâΩìxâÒì]Ç≥ÇπÇÈÇ©
	void RotateGraph(const float& deg)    { m_angle += deg * Geometry::kDegToRad; }
	// ë„ì¸
	void SetAngle(const float& angleDeg)  { m_angle = angleDeg * Geometry::kDegToRad; }
	// SetDrawBlendModeÇÇªÇÃÇ‹Ç‹ï\Ç…å©ÇπÇƒÇ¢ÇÈ
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

