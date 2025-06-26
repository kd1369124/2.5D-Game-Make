#include "ResurutScene.h"
#include "../SceneManager.h"
#include"Resurut.h"

void ResurutScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
		KdAudioManager::Instance().StopAllSound();
	}
}

void ResurutScene::Init()
{
	std::shared_ptr<Result> titleSprite = nullptr;
	titleSprite = std::make_shared<Result>();

	titleSprite->Init();
	m_objList.push_back(titleSprite);

	// BGM再生
	if (!m_bgm || !m_bgm->IsPlaying())
	{
		m_bgm = KdAudioManager::Instance().Play("Asset/Sounds/result.WAV", true);
		if (m_bgm)
		{
			m_bgm->SetVolume(0.1f);
		}
	}
}
