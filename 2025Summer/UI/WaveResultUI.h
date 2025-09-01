#pragma once
#include "UIBase.h"
#include "../GameManagement/Score/StylishEnum.h"
#include "../Utility/RangeLimitedValue.h"

class Image;
class ArenaMode;
struct WaveResult;

// ウェーブ間のリザルトを出します
class WaveResultUI : public UIBase
{
public:
	WaveResultUI();
	~WaveResultUI();

	void Init();

	void Update() override;

	void Draw() const override;

	// 今の状態からリザルト出す
	void StartResult();

private:

	Vector2 m_lerpStartPos;
	// 移動したり数字をカウントしたりするのに使いまわしてます
	RangeLimitedValue<float, 0.0f, 1.0f> m_lerpTime;

	int m_fontH;

	std::weak_ptr<WaveResult> m_resultData;

	using Sequence_t = void (WaveResultUI::*)();
	Sequence_t m_sequence;
	using DrawFunc_t = void (WaveResultUI::*)() const;
	DrawFunc_t m_draw;

private:

	// 冗長なのは許してください
	void Appear();
	void CountDamageAmount();
	void CountClearTime();
	void CountTotalStylishPoint();
	void Disappear();
	void Disable();

	void BoxDraw() const;
	void DamageDraw() const;
	void TimeDraw() const;
	void StylishPointDraw() const;
	void NoDraw() const;
};

