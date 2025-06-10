#pragma once
class Ground : public KdGameObject
{
public:
	Ground(){}
	~Ground(){}

	void Init()override;

	void Update()override;

	void DrawLit()override;
private:


};