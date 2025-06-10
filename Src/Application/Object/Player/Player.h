#pragma once
class Player : public KdGameObject
{
public:
	Player(){}
	~Player(){}

	void Init()override;

	void Update()override;
	void PostUpdate()override;

private:

};
