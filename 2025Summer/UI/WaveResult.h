#pragma once
#include "UIBase.h"
#include "StylishEnum.h"

class Image;
class ArenaMode;

// ウェーブ間のリザルトを出します
class WaveResult : public UIBase
{
public:
	WaveResult();
	~WaveResult();

	void Init();

	void Update() override;

	void Draw() const override;

	// 今の状態からリザルト出す
	void StartResult();

private:

	std::shared_ptr<Image> m_image;
	int m_fontH;

	// 被ダメ
	float m_damageAmount;
	// 時間
	float m_second;
	// 総合ランク
	StylishRankKind m_rank;

	using Sequence_t = void (WaveResult::*)();
	Sequence_t m_sequence;

private:

	void Appear();
	void Wait();
	void Disappear();
};

