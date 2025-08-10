#include "SkyBox.h"
#include "ShaderDraw.h"
#include "../UI/ImageStore.h"
#include <DxLib.h>
#include "../Camera/Camera.h"
#include <cassert>
#include "../Model/Model.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init(const std::string& fileName, std::weak_ptr<Camera> camera)
{
	m_camera = camera;

	m_model = std::make_shared<Model>();
	m_model->Init("Data/Model/SkyBox.mv1");
	m_model->SetWriteZBuffer(false);
}

void SkyBox::Update()
{
	m_model->SetPos(GetCameraPosition());
}

void SkyBox::Draw() const
{
	m_model->Draw();
}
