#pragma once
#include "Scene.h"
#include <memory>
#include "../Geometry/Vector2.h"
#include "../Geometry/Vector3.h"
#include <list>
#include "../UI/UIBase.h"

class Image;

// UIの位置を編集して出力します
class SceneUIEdit : public Scene
{
private:

	struct UIPreview
	{
	public:

		std::shared_ptr<Image> image;
		Vector2 pos;
		Vector3 scale;
	};

	// データを記録する処理をクラスに分けた
	class UIPositionRecorder
	{
	public:
		UIPositionRecorder();

		void operator()();
	};

public:
	SceneUIEdit();

	void Init() override;

	void Update() override;
	void Draw() const override;

private:

	std::list<std::shared_ptr<UIPreview>> m_previews;
	std::weak_ptr<UIPreview> m_nowSelectedPreview;

private:

	void OutputPosition();
};

