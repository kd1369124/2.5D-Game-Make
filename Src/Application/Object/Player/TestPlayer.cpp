﻿#include "TestPlayer.h"
#include "../../Scene/SceneManager.h"
#include"../Attack/Attack.h"
void TestPlayer::Init()
{
	//デバック用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	//初期状態では右を向く
	{
		Left = false;
		Right = true;
	}

	//待機状態Flgをtrueで初期化
	IdleFlg = false;

	//攻撃Flgをfalseで初期化
	AtkFlg = false;

	//キーフラグをfalseで初期化
	KeyFlg = false;

	//防御Flgをtrueで初期化
	GuardFlg = false;

	ZKeyFlg = false; //Zキーを押しているかのフラグをfalseで初期化
	ZmovepulsFlg = NULL;
	m_direction = true; // true:右, false:左

	DamageFlg = false;						// ダメージフラグを初期化

	//移動関係Flgをfalseで初期化
	{
		WalkFlg = false;
		RunFlg = false;
	}

	//プレイヤーの初期位置を{-55.0f,2.0f,0.0f}で初期化
	m_pos = { };
	m_move = {};

	//プレイヤーの大きさを2で初期化

	m_polygon = std::make_shared<KdSquarePolygon>();

	m_polygon->SetScale(2);

	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	//重力を0.05fで初期化
	m_gravity = 0.05f;

	m_HitArea = 0.7f; // 攻撃判定のエリアを0.3fで初期化

	//アニメーション初期化
	m_animeInfo.start = 0.0f;	//開始コマを0.0fで初期化
	m_animeInfo.end = 0.0f;		//終了コマを0.0fで初期化
	m_animeInfo.count = 0.0f;	//現在のコマ数カウントを0.0fで初期化
	m_animeInfo.speed = 0.0f;	//アニメーション速度を0.0fで初期化
	m_pCollider = std::make_unique<KdCollider>();


	m_pCollider->RegisterCollisionShape(
		"PlayerCollision",
		Math::Vector3(0, 0.5, 0),							// 球の中心座標（原点からの位置）
		m_HitArea,										// 球の半径
		KdCollider::TypeDamage								// 当たり判定のタイプ
	);

	Alive = true;

	m_hp = 500; // HPを500で初期化
	m_maxHp = 500; // 最大HPを500で初期化

	FrashFlg = false;	// フラッシュフラグを初期化

	DamageEndFlg = false;	// ダメージ終了フラグを初期化

	DamageFlg = false;	// ダメージフラグを初期化

	FlashTimer = 0;         // 点滅用フレームカウンターを初期化
	m_objectType = ObjctType::Player;
}

void TestPlayer::Update()
{
	m_dir = {};
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//待機状態の処理　ここから
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	{
		//A,D,スペース,左クリックを押していないとき
		if (!(GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState('D') & 0x8000) &&
			!(GetAsyncKeyState(VK_SPACE) & 0x8000) && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			//移動関係のFlgをfalseにする
			{
				WalkFlg = false;
				RunFlg = false;
			}
			//IdleFlgをtrueにする
			IdleFlg = true;
		}
		//IdleFlgがtrueなら
		if (IdleFlg)
		{
			//右向きなら
			if (Right)
			{
				//板ポリに張り付けてる画像をRightIdle.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Idle/RightIdle.png");
			}
			//左向きなら
			if (Left)
			{
				//板ポリに張り付けてる画像をLeftIdle.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Idle/LeftIdle.png");
			}
			//横6　縦１に画像を分割
			m_polygon->SetSplit(6, 1);
			//終了コマを6コマ目にする
			m_animeInfo.end = 5;
			//アニメーション速度を0.18fにする
			m_animeInfo.speed = 0.08f;
		}
	}
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//待機状態の処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//移動の処理　ここから
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	if ((GetAsyncKeyState('W') & 0x8000))
	{
		ZKeyFlg = true;
		ZmovepulsFlg = true;
		//シフトボタンと右クリックを押していないとき
		if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000) && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
		{
			//RunFlgをfalseにする
			RunFlg = false;
			//WalkFlgをtrueにする
			WalkFlg = true;
		}
		//またはシフトボタンと右クリックを押しているとき
		else if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
		{
			//WalkFlgをfalseにする
			WalkFlg = false;
			//RunFlgをtrueにする
			RunFlg = true;
		}
		//WalkFlgがtrueで左向きなら
		if (WalkFlg && Left)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をLeftWalk.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Walk/LeftWalk.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.08f;
			//m_pos.xに0.02fを引き続ける
			m_dir += {0, 0, 1};
		}
		//RunFlgがtrueで左向きなら
		if (RunFlg && Left)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightRun.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Run/LeftRun.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.15f;
			//m_pos.xに0.05fを引き続ける
			m_dir += {0, 0, 1};
		}
		if (WalkFlg && Right)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightWalk.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Walk/RightWalk.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.08f;
			//m_pos.xに0.02fを足し続ける
			m_dir += {0, 0, 1};
		}
		//RunFlgがtrueで右向きなら
		if (RunFlg && Right)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightRun.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Run/RightRun.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.15f;
			//m_pos.xに0.05fを足し続ける
			m_dir += {0, 0, 1};
		}
	}
	if ((GetAsyncKeyState('S') & 0x8000))
	{
		ZKeyFlg = true;
		ZmovepulsFlg = false;
		//シフトボタンと右クリックを押していないとき
		if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000) && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
		{
			//RunFlgをfalseにする
			RunFlg = false;
			//WalkFlgをtrueにする
			WalkFlg = true;
		}
		//またはシフトボタンと右クリックを押しているとき
		else if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
		{
			//WalkFlgをfalseにする
			WalkFlg = false;
			//RunFlgをtrueにする
			RunFlg = true;
		}
		if (WalkFlg && Right)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightWalk.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Walk/RightWalk.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.08f;

			m_dir += {0, 0, -1};
		}
		//RunFlgがtrueで右向きなら
		if (RunFlg && Right)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightRun.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Run/RightRun.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.15f;
			//m_pos.xに0.05fを足し続ける
			m_dir += {0, 0, -1};
		}
		if (WalkFlg && Left)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をLeftWalk.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Walk/LeftWalk.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.08f;
			//m_pos.xに0.02fを引き続ける
			m_dir += {0, 0, -1};
		}
		//RunFlgがtrueで左向きなら
		if (RunFlg && Left)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(8, 1);
			//板ポリに張り付けてる画像をRightRun.pngにする
			m_polygon->SetMaterial("Asset/Textures/Chara/Run/LeftRun.png");
			//終了コマを7コマ目にする
			m_animeInfo.end = 7;
			//アニメーション速度を0.08fにする
			m_animeInfo.speed = 0.15f;
			//m_pos.xに0.05fを引き続ける
			m_dir += {0, 0,-1};
		}
	}
	else if (!(GetAsyncKeyState('S') & 0x8000) && !(GetAsyncKeyState('W') & 0x8000))
	{
		ZKeyFlg = false; //Zキーを押しているフラグをfalseにする
		ZmovepulsFlg = NULL;
	}
	
		// 左方向への移動処理ここから
		//Aキーを押しているとき
		if ((GetAsyncKeyState('A') & 0x8000))
		{
			//IdleFlgをfalseにする
			IdleFlg = false;
			m_direction = false; // 向きを左に設定
			//向いている方向を左に設定
			{
				Left = true;
				Right = false;
			}
			//シフトボタンと右クリックを押していないとき
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000) && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
			{
				//RunFlgをfalseにする
				RunFlg = false;
				//WalkFlgをtrueにする
				WalkFlg = true;
			}
			//またはシフトボタンと右クリックを押しているとき
			else if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
			{
				//WalkFlgをfalseにする
				WalkFlg = false;
				//RunFlgをtrueにする
				RunFlg = true;
			}
			//WalkFlgがtrueで左向きなら
			if (WalkFlg && Left)
			{
				//横8　縦１に画像を分割
				m_polygon->SetSplit(8, 1);
				//板ポリに張り付けてる画像をLeftWalk.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Walk/LeftWalk.png");
				//終了コマを7コマ目にする
				m_animeInfo.end = 7;
				//アニメーション速度を0.08fにする
				m_animeInfo.speed = 0.08f;
				//m_pos.xに0.02fを引き続ける
				m_dir += {-1, 0, 0};
			}
			//RunFlgがtrueで左向きなら
			if (RunFlg && Left)
			{
				//横8　縦１に画像を分割
				m_polygon->SetSplit(8, 1);
				//板ポリに張り付けてる画像をRightRun.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Run/LeftRun.png");
				//終了コマを7コマ目にする
				m_animeInfo.end = 7;
				//アニメーション速度を0.08fにする
				m_animeInfo.speed = 0.15f;
				//m_pos.xに0.05fを引き続ける
				m_dir += {-1, 0, 0};
			}
		}
		else
		{
			//移動関係のFlgをfalseにする
			{
				WalkFlg = false;
				RunFlg = false;
			}
		}
		// 左方向への移動処理ここまで

		//右方向への処理ここから
		//Dキーを押しているとき
		if ((GetAsyncKeyState('D') & 0x8000))
		{
			//IdleFlgをfalseにする
			IdleFlg = false;
			m_direction = true; // 向きを右に設定
			//向いている方向を右に設定
			{
				Right = true;
				Left = false;
			}
			//シフトボタンと右クリックを押していないとき
			if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000) && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
			{
				//RunFlgをfalseにする
				RunFlg = false;
				//WalkFlgをtrueにする
				WalkFlg = true;
			}
			//またはシフトボタンと右クリックを押しているとき
			else if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
			{
				//WalkFlgをfalseにする
				WalkFlg = false;
				//RunFlgをtrueにする
				RunFlg = true;
			}
			//WalkFlgがtrueで右向きなら
			if (WalkFlg && Right)
			{
				//横8　縦１に画像を分割
				m_polygon->SetSplit(8, 1);
				//板ポリに張り付けてる画像をRightWalk.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Walk/RightWalk.png");
				//終了コマを7コマ目にする
				m_animeInfo.end = 7;
				//アニメーション速度を0.08fにする
				m_animeInfo.speed = 0.08f;
				//m_pos.xに0.02fを足し続ける
				m_dir += {1, 0, 0};
			}
			//RunFlgがtrueで右向きなら
			if (RunFlg && Right)
			{
				//横8　縦１に画像を分割
				m_polygon->SetSplit(8, 1);
				//板ポリに張り付けてる画像をRightRun.pngにする
				m_polygon->SetMaterial("Asset/Textures/Chara/Run/RightRun.png");
				//終了コマを7コマ目にする
				m_animeInfo.end = 7;
				//アニメーション速度を0.08fにする
				m_animeInfo.speed = 0.15f;
				//m_pos.xに0.05fを足し続ける
				m_dir += {1, 0, 0};
			}

			
		}
		else
		{
			//移動関係のFlgをfalseにする
			{
				WalkFlg = false;
				RunFlg = false;
			}
		}
		//右方向への処理ここまで

	
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//移動の処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//ジャンプの処理　ここから
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝



	//スペースキーを押したとき
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//ジャンプFlgをtrueにする
		if (!JumpFlg && !AtkFlg && !GuardFlg)
		{
			m_animeInfo.count = 0;
			JumpFlg = true; //ジャンプFlgをtrueにする
			m_gravity -= 0.1f; //重力を0にする
		}
	}

	//ジャンプFlgがtrueなら
	if (JumpFlg)
	{
		//左向きなら
		if (Left)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(3, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Jump/LeftJump.png");
			m_animeInfo.speed = 0.1f;
			m_animeInfo.end = 2;
		}
		//右向きなら
		if (Right)
		{
			//横8　縦１に画像を分割
			m_polygon->SetSplit(3, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Jump/RightJump.png");
			m_animeInfo.speed = 0.1f;
			m_animeInfo.end = 2;
		}
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//ジャンプの処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	



	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//攻撃の処理　ここから
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
		//左クリックを押したとき
	if (((GetAsyncKeyState('L') & 0x8000) || (GetAsyncKeyState('U') & 0x8000)) && !KeyFlg)
	{
		KeyFlg = true;
		//攻撃とジャンプのフラグがfalseなら
		if (!AtkFlg && !JumpFlg && !GuardFlg)
		{
			m_animeInfo.count = 0;	//コマ数をリセット
			AtkFlg = true;			//攻撃Flgをtrueにする
		}
	}
	if (!((GetAsyncKeyState('L') & 0x8000) || (GetAsyncKeyState('U'))))
	{
		KeyFlg = false;
	}
	//攻撃Flgがtrueなら
	if (AtkFlg)
	{
		//右向きなら
		if (Right)
		{
			//m_move.xを0にして動きを止める
			m_move.x = 0;
			//横8　縦１に画像を分割
			m_polygon->SetSplit(4, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Attack/RightAttack_1.png");
			m_animeInfo.speed = 0.2f;
			m_animeInfo.end = 3;
		}
		//左向きなら
		if (Left)
		{
			//m_move.xを0にして動きを止める
			m_move.x = 0;
			//横8　縦１に画像を分割
			m_polygon->SetSplit(4, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Attack/LeftAttack_1.png");
			m_animeInfo.speed = 0.2f;
			m_animeInfo.end = 3;
		}
	}
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//攻撃の処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//防御の処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (GetAsyncKeyState('F') & 0x8000)
	{
		//攻撃とジャンプのフラグがfalseなら
		if (!AtkFlg && !JumpFlg && !GuardFlg)
		{
			m_animeInfo.count = 0;	//コマ数をリセット
			GuardFlg = true;			//攻撃Flgをtrueにする
		}

	}
	if (!(GetAsyncKeyState('F') & 0x8000))
	{
		GuardFlg = false;			//攻撃Flgをtrueにする
	}
	if (GuardFlg)
	{
		//右向きなら
		if (Right)
		{
			//m_move.xを0にして動きを止める
			m_move.x = 0;
			//横8　縦１に画像を分割
			m_polygon->SetSplit(2, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Protection/RightProtection.png");
		}
		//左向きなら
		if (Left)
		{
			//m_move.xを0にして動きを止める
			m_move.x = 0;
			//横8　縦１に画像を分割
			m_polygon->SetSplit(2, 1);
			m_polygon->SetMaterial("Asset/Textures/Chara/Protection/LeftProtection.png");
		}
	}
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//防御の処理　ここまで
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	////アニメーションの更新作業
	m_animeInfo.count += m_animeInfo.speed;    //コマ数を加算

	int animeCnt = static_cast<int>(m_animeInfo.start + m_animeInfo.count);
	//最後のコマを表示したら戻してループさせる
	if (!JumpFlg && !AtkFlg)
	{
		if (animeCnt > m_animeInfo.end)
		{
			animeCnt = m_animeInfo.start;        //最初のコマに戻す
			m_animeInfo.count = 0.0f;            //コマ数をリセット
		}
	}
	if (JumpFlg)
	{
		if (animeCnt > m_animeInfo.end)
		{
			animeCnt = 2;        //最初のコマに戻す
			m_animeInfo.count = 2.0f;
		}
	}

	//攻撃中
	if (GuardFlg)
	{
		//コマ数のカウントが3コマ以上なら
		if (animeCnt >= 1)
		{
			animeCnt = 1;			//画像を3コマ目で固定
			m_animeInfo.count = 1;	//コマ数を3に固定
		}
	}
	//UV設定
	m_polygon->SetUVRect(animeCnt);

	if (WalkFlg)
	{
		m_speed = 0.03;
	}
	else if (RunFlg)
	{
		m_speed = 0.06;
	}
	m_dir.Normalize();

	//重力処理
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;//重力加速

	float rotY = 0.0f; //Y軸回転角度

	if (ZmovepulsFlg && ZKeyFlg)
	{
		if (m_direction)
		{
			rotY = -45.0f;
			m_dir += {0, 0, 1};//Zキーを押しているときは前に進む
		}
		if (!m_direction)
		{
			rotY = 314.5f; //Zキーを押しているときは45度回転
			m_dir += {0, 0, 1};//Zキーを押しているときは前に進む
		}
	}
	else if (!ZmovepulsFlg && ZKeyFlg)
	{
		if (m_direction)
		{
			rotY = 314.5f;
			m_dir += {0, 0, -1};//Zキーを押しているときは前に進む
		}
		if (!m_direction)
		{
			rotY = -45.0f; //Zキーを押しているときは45度回転
			m_dir += {0, 0, -1};//Zキーを押しているときは前に進む
		}
	}

	static int counat = 0;

	if (counat >= 3)
	{
		FrashFlg = false; //3回点滅したら点滅を終了
		DamageEndFlg = false; //ダメージ終了フラグをtrueにする
		counat = 0; //カウントをリセット
	}

	// 点滅処理
	if (FrashFlg)
	{
		FlashTimer++;
		DamageEndFlg = true; //ダメージ終了フラグをtrueにする
		// 10フレームごとに表示/非表示を切り替え
		if (FlashTimer % 10 == 0)
		{
			Visible = !Visible;
			counat++;
		}
	}
	else
	{
		// 通常表示
		Visible = true;
		FlashTimer = 0;
	}

	
	m_pos += m_dir * m_speed;

	//攻撃中
	if (AtkFlg)
	{

		//コマ数のカウントが3コマ以上なら
		if (animeCnt >= 3)
		{

			// 攻撃オブジェクトを出現させる座標を確定させる
			Math::Vector3 attackPos = {};
			attackPos = m_pos;			// プレイヤーの座標を基準にする	// 攻撃方向に0.4だけずらす
			// 攻撃オブジェクトを作成
			std::shared_ptr<Attack> attack;
			attack = std::make_shared<Attack>();
			attack->Init();									// 初期化
			attack->SetDirection(m_direction,ZmovepulsFlg);				// 向きをセット
			attack->SetPos(attackPos);						// 座標をセット
			SceneManager::Instance().AddObject(attack);		// シーンに追加
			
				
			
			animeCnt = 3;			//画像を3コマ目で固定
			m_animeInfo.count = 3;	//コマ数を3に固定
			AtkFlg = false;			//攻撃Flgをfalseにする
		}
	}
	
	Math::Matrix RotY = Math::Matrix::CreateRotationY(rotY);

	//プレイヤーの座標行列
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);

	//行列の合成
	m_mWorld = RotY * transMat;

}
void TestPlayer::PostUpdate()
{
	//========================
	//当たる側の処理
	//========================

	//========================
	//レイ判定
	//========================
	//レイ判定用に変数を作成
	KdCollider::RayInfo rayInfo;
	//レイの発射位置(座標)を設定
	rayInfo.m_pos = m_pos;
	//ちょっと上からの位置にする
	rayInfo.m_pos.y += 0.1f;
	//段差の許容範囲を設定
	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;
	//レイの発射方向
	rayInfo.m_dir = { 0,-1,0 }; //今回のみ下方向
	//レイの長さを設定
	rayInfo.m_range = m_gravity + enableStepHigh;
	//当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	//デバック
	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	//レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;

	//レイと当たり判定をする!!
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	//レイに一番近い
	float maxOverlap = 0.0f;
	Math::Vector3 hitPos;
	bool hit = false;

	for (auto& ret : retRayList)
	{
		//レイを遮断しオーバーした長さが一番長いものを探す
		if (maxOverlap < ret.m_overlapDistance)
		{
			//更新
			maxOverlap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			hit = true;
		}
	}
	if (hit)
	{
		//当たっていたら
		m_pos = hitPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0.0f;
		JumpFlg = false;

	}
	//========================
	//球判定
	//========================
	//球判定用変数
	KdCollider::SphereInfo spherInfo;
	//球の中心座標
	spherInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.65f, 0);
	//球の半径
	spherInfo.m_sphere.Radius = 0.5f;
	//当たり判定したいタイプを設定
	spherInfo.m_type = KdCollider::TypeGround;

	//デバック
	//m_pDebugWire->AddDebugSphere(spherInfo.m_sphere.Center, spherInfo.m_sphere.Radius);

	//球にあたったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	//球と当たり判定をする!!
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(spherInfo, &retSphereList);
	}

	//球にあたったリストから一番近いオブジェクトを検出
	float MaxOverlap = 0.0f;
	Math::Vector3 hitSphereDir;

	for (auto& ret : retSphereList)
	{
		if (MaxOverlap < ret.m_overlapDistance)
		{
			MaxOverlap = ret.m_overlapDistance;
			hitSphereDir = ret.m_hitDir;
			hit = true;
		}
	}
	if (hit)
	{
		//Z方向への押し出し
		hitSphereDir.z = 0.0f;

		hitSphereDir.Normalize();

		m_pos += hitSphereDir * MaxOverlap;
	}
}

void TestPlayer::DrawLit()
{
	if(Visible)KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
	
}

void TestPlayer::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void TestPlayer::onHit()
{
	if (DamageEndFlg || GuardFlg)return;	// ダメージ終了フラグが立っている場合は何もしない

	m_hp -= 30;
	if (m_hp <= 0)
	{
		Alive = false;					// HPが0以下ならAliveをfalseにする
	}
	else
	{
		DamageFlg = true;		// ダメージフラグを立てる
		FrashFlg = true;		// 点滅フラグを立てる
	}
}

