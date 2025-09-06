#pragma once
#include "../Actor.h"
#include <string>

class Model;

// ステージのモデルと当たり判定をクラス化する

class Field : public Actor
{
public:
	Field();

	void Init(const std::string& modelPath);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

private:

	std::shared_ptr<Model> m_model;

};

