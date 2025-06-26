#pragma once

#include"../BaseScene/BaseScene.h"

class ResurutScene : public BaseScene
{
public:

	ResurutScene() { Init(); }
	~ResurutScene() {}

private:

	void Event() override;
	void Init()  override;

	std::shared_ptr<KdSoundInstance> m_bgm;

};
