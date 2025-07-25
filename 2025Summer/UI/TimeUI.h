#pragma once
#include "Vector2.h"
#include <memory>
#include "Color.h"
#include "UIBase.h"

class Timer;

// 制限時間を表示するだけ
class TimeUI : public UIBase
{
public:
	TimeUI();
	~TimeUI();

	void Init(const Vector2& initPos, std::weak_ptr<Timer> timerRef);

	void Update() override;

	void Draw() const override;

	// 一応あとからでも位置を変更できる
	void SetPos(const Vector2& pos) { m_pos = pos; }
private:
	Vector2 m_pos;
	// ○.○○秒
	float m_second;
	std::weak_ptr<Timer> m_timerRef;
	int m_fontHandle;
	Color::ColorCode_t m_color;

private:
	// タイマーから時間を取得(秒数)
	void GetTime();
};

