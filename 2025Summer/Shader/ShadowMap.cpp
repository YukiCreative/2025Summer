#include "ShadowMap.h"
#include <DxLib.h>

namespace
{
	constexpr int kShadowTextureSize = 1024;
}

ShadowMap::ShadowMap() :
	m_handle(-1)
{
}

ShadowMap::~ShadowMap()
{
	// ƒnƒ“ƒhƒ‹‚ğíœ
	DeleteShadowMap(m_handle);
}

void ShadowMap::Init(const Vector3& areaMin, const Vector3& areaMax)
{
	m_handle = MakeShadowMap(kShadowTextureSize, kShadowTextureSize);
	SetShadowMapDrawArea(m_handle, areaMin, areaMax);
}

void ShadowMap::StartShadowMapDraw(const Vector3& lightDir)
{
	ShadowMap_DrawSetup(m_handle);
	SetShadowMapLightDirection(m_handle, lightDir.GetNormalize());
}

void ShadowMap::EndShadowMapDraw()
{
	ShadowMap_DrawEnd();
	SetUseShadowMap(0, m_handle);
}

void ShadowMap::UnsetShadowMap()
{
	SetUseShadowMap(0, -1);
}
