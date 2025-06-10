#include "GameScene.h"
#include"../SceneManager.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Ground/Ground.h"
#include"../../Object/BackGround/NomalBack.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
}

void GameScene::Init()
{
	//==============プレイヤー初期化===================
	player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	//=================================================

	//===============マップ地面初期化==================
	ground = std::make_shared<Ground>();
	ground->Init();
	m_objList.push_back(ground);
	//=================================================

	//===============通常背景初期化====================
	nBack = std::make_shared<NomalBack>();
	nBack->Init();
	m_objList.push_back(nBack);
	//=================================================
}
