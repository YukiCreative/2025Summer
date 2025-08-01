#pragma once
#include "../Geometry/Vector3.h"
#include <memory>
#include <unordered_map>
#include <list>
#include <string>

class EffekseerEffect;

// Effekseerを使って
// エフェクトを出す
// エフェクトを動かす
// エフェクトを描画
// シングルトンにしよ
class EffectManager
{
private:
	EffectManager();
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;

public:
	~EffectManager();

	static EffectManager& GetInstance();

	void Init();

	void Update();
	void Draw() const;

	// ポインタを返して呼び出したクラスが操作できるようにする
	std::weak_ptr<EffekseerEffect> GenerateEffect(const std::string& fileName, const Vector3& initPos);
	// 今出ているエフェクトをすべて削除
	void ClearEffects();
private:
	using EffectSourceMap_t = std::unordered_map<std::string, int>;
	EffectSourceMap_t m_effectSources;

	using EffectList_t = std::list<std::shared_ptr<EffekseerEffect>>;
	EffectList_t m_effects;

private:

	void LoadEffectSources();
};