#include "SceneTest.h"
#include <DxLib.h>
#include "Input.h"

SceneTest::SceneTest()
{
}

void SceneTest::Init()
{
}

void SceneTest::Update()
{
}

void SceneTest::Draw() const
{
	Input& input = Input::GetInstance();
	const Vector2 left = input.GetLeftInputAxis();
	const Vector2 right = input.GetRightInputAxis();
	DrawFormatString(0,0,0xffffff, "左スティック x:%f,y:%f",left.x, left.y);
	DrawFormatString(0,15,0xffffff, "右スティック x:%f,y:%f",right.x, right.y);

	if (input.IsPressed("Right"))
	{
		DrawString(0,30,"右押されてます", 0xffffff);
	}
}

void SceneTest::Entry()
{
}

void SceneTest::Reave()
{
}