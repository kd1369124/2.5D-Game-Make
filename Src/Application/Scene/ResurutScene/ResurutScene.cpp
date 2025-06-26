#include "ResurutScene.h"
#include "../SceneManager.h"
#include"Resurut.h"

void ResurutScene::Event()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
}

void ResurutScene::Init()
{
	std::shared_ptr<Result> titleSprite = nullptr;
	titleSprite = std::make_shared<Result>();

	titleSprite->Init();
	m_objList.push_back(titleSprite);
}
