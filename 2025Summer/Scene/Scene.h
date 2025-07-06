#pragma once

class Scene abstract
{
public:
	virtual ~Scene() = default;

	virtual void Init() abstract;
	virtual void Update() abstract;
	virtual void Draw() const abstract;

	virtual void Entry() abstract;
	virtual void Reave() abstract;
private:
};

