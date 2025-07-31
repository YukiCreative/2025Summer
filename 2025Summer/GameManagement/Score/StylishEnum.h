#pragma once

// 加点行動
enum class IncreaseStylishPointKind
{
	kNone,
	kSlashDown,
	kSlashLeftSide,
	kSlashRightSide,
	kSlashUp,
	kFinishAttack,
	kChargeAttack,
	kShockWaveAttack,
	kShockWave,
	kSpecialAttack,
};

enum class StylishRankKind
{
	kNoRank, // 初期状態　ランクの表示なし
	kDRank,
	kCRank,
	kBRank,
	kARank,
	kSRank,
	kSSRank,
	kSSSRank
};