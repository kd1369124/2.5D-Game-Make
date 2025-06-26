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
		KdAudioManager::Instance().StopAllSound();
	}
}

void TitleScene::Init()
{
	std::shared_ptr<Title> titleSprite = nullptr;
	titleSprite = std::make_shared<Title>();

	titleSprite->Init();
	m_objList.push_back(titleSprite);

	// BGM再生
	if (!m_bgm || !m_bgm->IsPlaying())
	{
		m_bgm = KdAudioManager::Instance().Play("Asset/Sounds/title.WAV", true);
		if (m_bgm)
		{
			m_bgm->SetVolume(0.1f);
		}
	}

}
