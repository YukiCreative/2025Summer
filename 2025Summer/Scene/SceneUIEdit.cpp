#include "SceneUIEdit.h"
#include "../UI/Image.h"
#include "../General/Input.h"
#include <fstream>
#include <cassert>

SceneUIEdit::SceneUIEdit()
{
}

void SceneUIEdit::Init()
{
}

void SceneUIEdit::Update()
{
	auto& input = Input::GetInstance();

	// クリックして画像を動かす
	if ()
	{

	}

	// 保存キーを押したら今出ているUIの座標を保存
	if (Input::GetInstance().IsTrigger("Submit"))
	{
		OutputPosition();
	}
}

void SceneUIEdit::Draw() const
{
	DrawString(0,0, "決定キーで保存します", 0xffffff);

	for (auto& preview : m_previews)
	{
		preview->image->Draw(preview->pos);
	}
}

void SceneUIEdit::OutputPosition()
{
	UIPositionRecorder recorder;

	recorder();
}

// ====================================================

namespace
{
	const std::string kFilePath = "Data/UIPositionData/UIPositionData.dat";
}

SceneUIEdit::UIPositionRecorder::UIPositionRecorder()
{

}

void SceneUIEdit::UIPositionRecorder::operator()()
{
}