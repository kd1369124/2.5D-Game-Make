#include "GameScene.h"
#include"../SceneManager.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Ground/Ground.h"
#include"../../Object/BackGround/NomalBack.h"
#include"../../Object/Enemy/Enemy.h"
#include"../../Object/Ground/Ground.h"
#include"../../Load/JsonLoad.h"

void GameScene::Event()
{
	Math::Vector3 playerPos = {};
	playerPos = player->GetPos();	// プレイヤーの位置を取得

	// カメラの座標行列
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 3.0f, -5.0f) + playerPos);

	// カメラの回転行列を作成
	// 少し下を向かせる
	Math::Matrix rotMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(25));

	// 行列を合成（拡縮 * 回転 * 座標）
	Math::Matrix mat = rotMat * transMat;

	// カメラに行列をセット
	// この時点では画面に反映されない
	m_camera->SetCameraMatrix(mat);
	
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

	//===============カメラ初期化=====================
	m_camera = std::make_unique<KdCamera>();			// 1 メモリ確保
	m_camera->SetProjectionMatrix(60);					// 2 視野角設定

	//=================================================

	//=================================================

		//===============ノード読み込み=====================
	auto nodes = JsonNodeLoader::LoadNodes("Asset/Data/nodes.json"); 

	for (const auto& node : nodes)
	{
		std::shared_ptr<KdGameObject> obj;

		// プレイヤー初期位置
		if (node.name == "PlayerSpawn")
		{
			player = std::make_shared<Player>();
			player->SetPos(node.pos); // ← JSONで読み取った位置に設定
			player->Init();
			obj = player;
		}
		// エネミー出現位置
		else if (node.name.rfind("Enemy", 0) == 0) // "Enemy" で始まる名前
		{
			std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
			enemy->SetPos(node.pos);
			enemy->Init();
			obj = enemy;
		}

		if (obj) { m_objList.push_back(obj); }
	}
	//=================================================

	//=================================================


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

	//===============メイン雑魚エネミー================
	 
	
	//=================================================

	//===============マップ地面初期化==================

	

	//=================================================

	
}
