#pragma once
#include <memory>

class Model;
class AnimationModel;

namespace ShaderDraw
{
	void DrawModel(std::shared_ptr<Model> model, int psH = -1, int vsH = -1, int texH = -1, int texH2 = -1);
	void DrawModel(std::shared_ptr<AnimationModel> model, int psH = -1, int vsH = -1, int texH = -1);

	// 画像はスクリーンの形に引き延ばされる
	void DrawScreen(const int tex, const int psH, const int tex2 = -1);
}