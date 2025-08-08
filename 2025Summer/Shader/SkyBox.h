#pragma once

class SkyBox
{
public:
	SkyBox();

	void Init();
	void Update();
	void Draw() const;

	void SetGraph(const int handle);

private:

	int m_psH;


};

