#pragma once
#include <unordered_map>
#include <memory>
#include "EnemyKind.h"

// 敵のモデルハンドルを一括で読み込んで管理するクラス
// Generatorから分けた
class EnemyModelList
{
public:
	EnemyModelList();
	~EnemyModelList();

	void Init();

	// 既に登録されていたら古いほうを解放します
	void AddEnemyHandle(const EnemyKind kind, const int handle);
	// Duplicateしたものを返します
	const int GetEnemyHandle(const EnemyKind kind);

private:
	std::unordered_map<EnemyKind, int> m_handles;
private:
	void ClearHandle();
};

