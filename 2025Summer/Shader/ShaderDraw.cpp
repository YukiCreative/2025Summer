#include "ShaderDraw.h"

#include "AnimationModel.h"
#include "Model.h"
#include <DxLib.h>

void ShaderDraw::DrawModel(std::shared_ptr<Model> model, int psH, int vsH, int texH, int texH2)
{
	MV1SetUseOrigShader(true);

	if (psH  != -1) SetUsePixelShader(psH);
	if (vsH  != -1) SetUseVertexShader(vsH);
	if (texH != -1) SetUseTextureToShader(3, texH);
	if (texH2 != -1) SetUseTextureToShader(4, texH2);

	model->Draw();

	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetUseTextureToShader(3, -1);
	SetUseTextureToShader(4, -1);

	MV1SetUseOrigShader(false);
}

void ShaderDraw::DrawModel(std::shared_ptr<AnimationModel> model, int psH, int vsH, int texH)
{
	MV1SetUseOrigShader(true);

	if (psH != -1) SetUsePixelShader(psH);
	if (vsH != -1) SetUseVertexShader(vsH);
	if (texH != -1) SetUseTextureToShader(4, texH);

	model->Draw();

	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	SetUseTextureToShader(4, -1);

	MV1SetUseOrigShader(false);
}