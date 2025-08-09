#pragma once
#include <string>
#include <memory>

class Camera;
class Model;

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Init(const std::string& fileName, std::weak_ptr<Camera> camera);
	void Update();
	void Draw() const;

private:

	std::weak_ptr<Camera> m_camera;

	std::shared_ptr<Model> m_model;
};

