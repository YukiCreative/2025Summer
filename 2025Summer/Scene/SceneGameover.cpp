#include "SceneGameover.h"
#include "SceneResult.h"
#include <string>
#include "../General/Game.h"
#include "../Shader/ShaderDraw.h"
#include <DxLib.h>
#include "../General/Input.h"
#include "SceneController.h"
#include "../Sound/Music.h"
#include "../Sound/SoundManager.h"

namespace
{
	const std::string kImgName = "Data/Image/Gameover.png";
	const std::string kPSPath = "Data/Shader/GameoverDissolve.pso";
	const std::string kDissolvePath = "Data/Image/pattern.png";

	constexpr float kDissolveSpeed = 0.02f;

	const Vector2 kTextPos = {1000.0f, 700.0f};

	const std::string kBGMPath = "Data/BGM/Gameover.wav";
	const std::string kSoundName = "Gameover.wav";
}

SceneGameover::SceneGameover() :
	m_psH(-1),
	m_cBuff(nullptr),
	m_cBuffH(-1),
	m_imgH(-1),
	m_dissolveH(-1)
{
}

SceneGameover::~SceneGameover()
{
	DeleteShader(m_psH);
	DeleteShaderConstantBuffer(m_cBuffH);
	DeleteGraph(m_imgH);
	DeleteGraph(m_dissolveH);
}

void SceneGameover::Init()
{
	m_imgH = LoadGraph(kImgName.c_str());
	m_dissolveH = LoadGraph(kDissolvePath.c_str());

	m_psH = LoadPixelShader(kPSPath.c_str());
	m_cBuffH = CreateShaderConstantBuffer(sizeof(CBuff));
	m_cBuff = static_cast<CBuff*>(GetBufferShaderConstantBuffer(m_cBuffH));

	SoundManager::GetInstance().Play(kSoundName);
	Music::GetInstance().Play(kBGMPath);
}

void SceneGameover::Update()
{
	m_cBuff->time += kDissolveSpeed;
	m_cBuff->time = std::min(m_cBuff->time, 1.2f);

	UpdateShaderConstantBuffer(m_cBuffH);

	if (Input::GetInstance().IsTrigger("Submit"))
	{
		SceneController::GetInstance().ResetSceneWithFade(std::make_shared<SceneResult>());
	}
}

void SceneGameover::Draw() const
{
	SetShaderConstantBuffer(m_cBuffH, DX_SHADERTYPE_PIXEL, 4);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	ShaderDraw::DrawScreen(m_imgH, m_psH, m_dissolveH);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(static_cast<int>(kTextPos.x), static_cast<int>(kTextPos.y), "åàíËÉ{É^ÉìÇ≈éüÇ÷êiÇﬁ", 0xffffff);
}

void SceneGameover::Entry()
{
}

void SceneGameover::Reave()
{
}
