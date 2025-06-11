#include "Model.h"
#include <DxLib.h>
#include "Geometry.h"

Model::Model() :
	m_isLoadModel(false)
{
}

Model::~Model()
{
	// Ç‡Çµé©ï™Ç≈ÉçÅ[ÉhÇµÇΩÇ‡ÇÃÇ»ÇÁè¡Ç∑
	if (m_isLoadModel)
	{
		MV1DeleteModel(m_handle);
	}
}

void Model::Init(const int duplicateHandle)
{
	m_handle = MV1DuplicateModel(duplicateHandle);
}

void Model::Init(const std::string& path)
{
	m_isLoadModel = true;
	m_handle = MV1LoadModel(path.c_str());
}

void Model::SetPos(const Vector3& pos) const
{
	MV1SetPosition(m_handle, pos);
}

void Model::CommitMove(const Vector3& moveValue)
{
}

void Model::Draw() const
{
	MV1DrawModel(m_handle);
}

void Model::RotateUpVecY(const float rad)
{
	const Vector3 nowRotate = MV1GetRotationXYZ(m_handle);
	MV1SetRotationXYZ(m_handle, nowRotate + Vector3{0, rad, 0});
}

void Model::RotateUpVecZ(const float rad)
{
	const Vector3 nowRotate = MV1GetRotationXYZ(m_handle);
	MV1SetRotationXYZ(m_handle, nowRotate + Vector3{ 0, 0, rad });
}

void Model::SetMatrix(DxLib::tagMATRIX mat)
{
	MV1SetMatrix(m_handle, mat);
}

void Model::SetScale(const Vector3& scale)
{
	MV1SetScale(m_handle, scale);
}

DxLib::tagMATRIX Model::GetMatrix() const
{
	return MV1GetMatrix(m_handle);
}
