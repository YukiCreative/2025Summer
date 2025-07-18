#include "Model.h"
#include <DxLib.h>
#include "Geometry.h"
#include <cassert>

Model::Model() :
	m_handle(-1)
{
}

Model::~Model()
{
	MV1DeleteModel(m_handle);
}

void Model::Init(const int duplicateHandle)
{
	m_handle = MV1DuplicateModel(duplicateHandle);
}

void Model::Init(const std::string& path)
{
	m_handle = MV1LoadModel(path.c_str());
	assert(m_handle != -1 && "指定のパスにモデルがなかったよ");
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

DxLib::tagMATRIX Model::GetFrameMatrix(const std::string frameName) const
{
	return MV1GetFrameLocalWorldMatrix(m_handle, MV1SearchFrame(m_handle, frameName.c_str()));
}

Vector3 Model::GetFramePosition(const std::string& frameName) const
{
	return MV1GetFramePosition(m_handle, MV1SearchFrame(m_handle, frameName.c_str()));
}

const int Model::GetFrameNum() const
{
	return MV1GetFrameNum(m_handle);
}

const Vector3& Model::GetModelBBMin() const
{
	const int frameNum = GetFrameNum();
	Vector3 minPos = MV1GetFrameMinVertexLocalPosition(m_handle, 0);
	for (int i = 1; i < frameNum; ++i)
	{
		const Vector3 frameMinPos = MV1GetFrameMinVertexLocalPosition(m_handle, i);
		minPos.x = std::min(minPos.x, frameMinPos.x);
		minPos.y = std::min(minPos.y, frameMinPos.y);
		minPos.z = std::min(minPos.z, frameMinPos.z);
	}
	return minPos;
}

const Vector3& Model::GetModelBBMax() const
{
	const int frameNum = GetFrameNum();
	Vector3 maxPos = MV1GetFrameMaxVertexLocalPosition(m_handle, 0);
	for (int i = 1; i < frameNum; ++i)
	{
		const Vector3 frameMaxPos = MV1GetFrameMaxVertexLocalPosition(m_handle, i);
		maxPos.x = std::max(maxPos.x, frameMaxPos.x);
		maxPos.y = std::max(maxPos.y, frameMaxPos.y);
		maxPos.z = std::max(maxPos.z, frameMaxPos.z);
	}
	return maxPos;
}
