#pragma once
#include"../BaseScene/BaseScene.h"
class SelectScene : public BaseScene
{
public:
	SelectScene(){}
	~SelectScene()override{}

private:

	void Event() override;
	void Init()  override;

};
