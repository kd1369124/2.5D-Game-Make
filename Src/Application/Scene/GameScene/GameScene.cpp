#include "GameScene.h"
#include"../SceneManager.h"
//#include"../../Object/Player/Player.h"
#include"../../Object/Ground/Ground.h"
#include"../../Object/BackGround/NomalBack.h"
#include"../../Object/Enemy/Enemy.h"
#include"../../Load/JsonLoad.h"
#include"../../Object/Player/TestPlayer.h"
#include"../../Object/Enemy/ShotEnemy.h"
#include"../../../Framework/GameObject/KdGameObject.h"

void GameScene::Event()
{

	//カメラ制御
	{
			Math::Matrix _Scale =
				Math::Matrix::CreateScale(1);
			Math::Matrix _RotationX =
				Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(30));

			Math::Matrix _RotationY =
				Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));

			Math::Matrix _RotationZ =
				Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));

			Math::Vector3 camPos = { 1 + testplayer->GetPos().x ,2  , -2 + testplayer->GetPos().z};
			Math::Matrix _Trans =
				Math::Matrix::CreateTranslation(camPos);

			//行列を合成（ 拡縮 ＊ 回転 ＊ 座標 ）
			Math::Matrix _Mat =
				_Scale * (_RotationX * _RotationY * _RotationZ )* _Trans;

			//カメラに行列をセット
			//この視点では画面には反映されない
			m_camera->SetCameraMatrix(_Mat);
		

	}
	
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
	//================================================

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
	


	//===============ノード読み込み=====================
	//auto nodes = JsonNodeLoader::LoadNodes("Asset/JsonData/nodes2.json"); 

	//for (const auto& node : nodes)
	//{
	//	std::shared_ptr<KdGameObject> obj;

	//	// プレイヤー初期位置
	//	if (node.name == "SteageGoal")
	//	{
	//		testplayer = std::make_shared<TestPlayer>();
	//		testplayer->SetPos(node.pos * ground->GetScale());	// ← JSONで読み取った位置に設定
	//		testplayer->Init();
	//
	//		obj = testplayer;
	//	}
	//	// エネミー出現位置
	//	else if (node.name.rfind("Enemy", 0) == 0)				// "Enemy" で始まる名前
	//	{
	//		std::shared_ptr<ShotEnemy> shotenemy = std::make_shared<ShotEnemy>();
	//		shotenemy->Init();
	//		shotenemy->SetTarget(testplayer);					// ターゲットをプレイヤーに設定
	//		shotenemy->SetPos(node.pos);
	//		obj = shotenemy;
	//	}

	//	if (obj) { m_objList.push_back(obj); }
	//}
	//=================================================



	
		// ノード情報の読み込み
		auto nodes = JsonNodeLoader::LoadNodeS("Asset/Models/nodes.json");

		for (const auto& node : nodes)
		{
			// プレイヤー配置
			if (node.name == "SteageGoal")
			{
				testplayer = std::make_shared<TestPlayer>();
				testplayer->Init();
				testplayer->SetPos((Math::Vector3(node.pos.x, node.pos.z, node.pos.y)) * ground->GetScale());
				AddObject(testplayer);
			}
			// ゴールオブジェクト配置（必要に応じて）
			else if (node.name == "")
			{
				// ゴール用オブジェクトを追加可能
			}
			// エネミー配置
			else if (node.name.rfind("Enemy", 0) == 0)
			{
				std::shared_ptr<KdGameObject> enemy;

				if (node.type == "shot")
				{
					enemy = std::make_shared<ShotEnemy>();
				}
				else if (node.type == "Dash")
				{
					//enemy = std::make_shared<DashEnemy>();
				}
				else
				{
					//enemy = std::make_shared<Enemy>(); // fallback
				}

				if (enemy)
				{

					enemy->Init();
					enemy->SetPos((Math::Vector3(node.pos.x,node.pos.z,node.pos.y) * ground->GetScale()));
					m_objList.push_back(enemy);
				}
			}
		}
	int a = 0;
	
	for (const auto& obj : m_objList)
	{
		if (obj->GetObjectType() == KdGameObject::ObjctType::Enemy)
		{
			obj->SetTarget(testplayer);
		}
		else if (obj->GetObjectType() == KdGameObject::ObjctType::Player)
		{
			
		}
	}

	//==============プレイヤー初期化===================
	//player = std::make_shared<Player>();
	//player->Init();
	//m_objList.push_back(player);
	//=================================================



	//===============メイン雑魚エネミー================
	 
	
	//=================================================

	//===============マップ地面初期化==================

	

	//=================================================

	
}
