#include "TitleScene.h"
#include "../SceneManager.h"
#include"../../Titlesprite/Titlesprite.h"
void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}
}

void TitleScene::Init()
{
	std::shared_ptr<Title> titleSprite = nullptr;
	titleSprite = std::make_shared<Title>();

	titleSprite->Init();
	m_objList.push_back(titleSprite);

}
