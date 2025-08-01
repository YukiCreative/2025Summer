#pragma once
#include "../Geometry/Vector3.h"

//DxLibの影機能を使いやすくしたい
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Init(const Vector3& areaMin, const Vector3& areaMax);

	void StartShadowMapDraw(const Vector3& lightDir);

	// この間にDrawをすると影のほうに送られる

	void EndShadowMapDraw();

	// ここで通常通りのモデルの描画

	void UnsetShadowMap();

	// この後は再びStartするまで影なしの描画

private:

	int m_handle;
};

