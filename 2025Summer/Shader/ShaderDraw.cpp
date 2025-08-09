#include "ShaderDraw.h"

#include "../Model/AnimationModel.h"
#include "../Model/Model.h"
#include <DxLib.h>
#include <array>
#include "../General/Game.h"

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

void ShaderDraw::DrawScreen(const int tex, const int psH, const int tex2)
{
	const int kVertNum = 4;
	std::array<VERTEX2DSHADER, 4> vertices{};

	for (auto& vertex : vertices)
	{
		vertex.rhw = 1.0f;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.spc = GetColorU8(0, 0, 0, 0);
	}

	vertices[0].pos = { 0,0,0 };
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	vertices[0].su = 0.0f;
	vertices[0].sv = 0.0f;

	vertices[1].pos = { Game::kScreenWidth,0,0 };
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	vertices[1].su = 1.0f;
	vertices[1].sv = 0.0f;

	vertices[2].pos = { 0,Game::kScreenHeight,0 };
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;
	vertices[2].su = 0.0f;
	vertices[2].sv = 1.0f;

	vertices[3].pos = { Game::kScreenWidth,Game::kScreenHeight,0 };
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;
	vertices[3].su = 1.0f;
	vertices[3].sv = 1.0f;

	unsigned short index[6] = {0,1,2,2,1,3};

	SetUsePixelShader(psH);
	SetUseTextureToShader(3, tex);
	if (tex2 != -1) SetUseTextureToShader(4, tex2);

	const int kPolyNum = 2;
	DrawPolygonIndexed2DToShader(vertices.data(), static_cast<int>(vertices.size()), index, kPolyNum);

	SetUsePixelShader(-1);
	SetUseTextureToShader(3, -1);
	SetUseTextureToShader(4, -1);
}
