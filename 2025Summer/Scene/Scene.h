#pragma once

class Scene
{
public:

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

	virtual void Entry() = 0;
	virtual void Reave() = 0;
private:
};

