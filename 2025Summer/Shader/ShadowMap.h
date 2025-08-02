#pragma once
#include "../Geometry/Vector3.h"

//DxLib‚Ì‰e‹@”\‚ğg‚¢‚â‚·‚­‚µ‚½‚¢
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Init(const Vector3& areaMin, const Vector3& areaMax);

	void StartShadowMapDraw(const Vector3& lightDir);

	// ‚±‚ÌŠÔ‚ÉDraw‚ğ‚·‚é‚Æ‰e‚Ì‚Ù‚¤‚É‘—‚ç‚ê‚é

	void EndShadowMapDraw();

	// ‚±‚±‚Å’Êí’Ê‚è‚Ìƒ‚ƒfƒ‹‚Ì•`‰æ

	void UnsetShadowMap();

	// ‚±‚ÌŒã‚ÍÄ‚ÑStart‚·‚é‚Ü‚Å‰e‚È‚µ‚Ì•`‰æ

private:

	int m_handle;
};

