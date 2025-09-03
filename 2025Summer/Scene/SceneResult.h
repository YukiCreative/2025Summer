#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "../Geometry/Vector2.h"

class UIController;
class Image;
enum class WaveRank;
class AnimationModel;

class SceneResult : public Scene
{
public:
	SceneResult();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() const override;

	virtual void Entry() override;
	virtual void Reave() override;

private:

	std::shared_ptr<UIController> m_UI;
	std::shared_ptr<Image> m_resultTextImage;
	std::shared_ptr<Image> m_wavesTextImage;
	std::shared_ptr<Image> m_totalTextImage;
	std::vector<WaveRank> m_waveRanks;
	std::vector<std::shared_ptr<Image>> m_waveRankImages;
	std::shared_ptr<Image> m_totalRankImg;

	int m_backGroundH;

	int m_fontH;

	using ResultSequence_t = void (SceneResult::*)();
	ResultSequence_t m_sequence;

	std::shared_ptr<AnimationModel> m_trophyModel;

	int m_frame;

private:

	void ShowWaveRank();
	// トータルのランクを出す
	void ShowTotalRank();
	void ShowRankTrophy();
	void Wait();
};

