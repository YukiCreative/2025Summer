#include "ShaderDraw.h"

#include "AnimationModel.h"
#include "Model.h"
#include <DxLib.h>

void ShaderDraw::DrawModel(std::shared_ptr<Model> model, int psH, int vsH, int texH)
{
	MV1SetUseOrigShader(true);

	if (psH  != -1) SetUsePixelShader(psH);
	if (vsH  != -1) SetUseVertexShader(vsH);
	if (texH != -1) SetUseTextureToShader(0, texH);

	model->Draw();

	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetUseTextureToShader(0, -1);

	MV1SetUseOrigShader(false);
}

void ShaderDraw::DrawModel(std::shared_ptr<AnimationModel> model, int psH, int vsH, int texH)
{
	MV1SetUseOrigShader(true);

	if (psH != -1) SetUsePixelShader(psH);
	if (vsH != -1) SetUseVertexShader(vsH);
	if (texH != -1) SetUseTextureToShader(0, texH);

	model->Draw();

	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetUseTextureToShader(0, -1);

	MV1SetUseOrigShader(false);
}