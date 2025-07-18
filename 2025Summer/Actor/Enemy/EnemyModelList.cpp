#include "EnemyModelList.h"
#include <DxLib.h>

EnemyModelList::EnemyModelList()
{
}

void EnemyModelList::Init()
{

}

void EnemyModelList::AddEnemyHandle(const EnemyKind kind, const int handle)
{
	if (m_handles.contains(kind))
	{
		MV1DeleteModel(handle);
	}
	m_handles[kind] = handle;
}

const int EnemyModelList::GetEnemyHandle(const EnemyKind kind)
{
	return MV1DuplicateModel(m_handles[kind]);
}
