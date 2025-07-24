#pragma once
#include <unordered_map>
#include <memory>
#include "EnemyKind.h"

// �G�̃��f���n���h�����ꊇ�œǂݍ���ŊǗ�����N���X
// Generator���番����
class EnemyModelList
{
public:
	EnemyModelList();
	~EnemyModelList();

	void Init();

	// ���ɓo�^����Ă�����Â��ق���������܂�
	void AddEnemyHandle(const EnemyKind kind, const int handle);
	// Duplicate�������̂�Ԃ��܂�
	const int GetEnemyHandle(const EnemyKind kind);

private:
	std::unordered_map<EnemyKind, int> m_handles;
private:
	void ClearHandle();
};

