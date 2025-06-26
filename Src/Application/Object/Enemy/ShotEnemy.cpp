#include "ShotEnemy.h"
#include"../../Scene/SceneManager.h"
#include"../../Object/Player/TestPlayer.h"
#include"../../Object/Attack/EnemyAttack.h"

namespace TexturePath
{
	const std::string Idle		= "Asset/Textures/Ninja_Peasant/Idle.png";
	const std::string Walk		= "Asset/Textures/Ninja_Peasant/Walk.png";
	const std::string Jump		= "Asset/Textures/Ninja_Peasant/Jump.png";
	const std::string Run		= "Asset/Textures/Ninja_Peasant/Run.png";
	const std::string Dead		= "Asset/Textures/Ninja_Peasant/Dead.png";
	const std::string Attack1	= "Asset/Textures/Ninja_Peasant/Attack_1.png";
	const std::string Attack2	= "Asset/Textures/Ninja_Peasant/Attack_2.png";
	const std::string Attack3	= "Asset/Textures/Ninja_Peasant/Shot.png";
	const std::string Damege	= "Asset/Textures/Ninja_Peasant/Hurt.png";
}
void ShotEnemy::Init()
{
	m_polygon = std::make_shared<KdSquarePolygon>();	// ポリゴンを生成
	m_polygon->SetMaterial(TexturePath::Idle);						// マテリアルを設定
	m_polygon->SetSplit(6, 1);
	m_polygon->SetScale(1.5);											// ポリゴンのスケールを設定
	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);	// ポリゴンのピボットを設定

	m_pos				= {};							// 初期位置を設定
	m_gravity			= 0.5f;							// 重力の値を設定
	m_mWorld			= Math::Matrix::Identity;		// ワールド行列を単位行列で初期化
	m_ShotArea			= 7.0f;							// ショット攻撃範囲の半径を設定
	m_searchArea		= 4.0f;							// プレイヤー検知範囲の半径を設定
	m_dashthArea		= 1.5f;							// ダッシュ攻撃範囲の半径を設定
	m_attackarea		= 0.7f;							// 攻撃範囲の半径を設定
	m_speed				= 0.04f;						// 移動速度を設定
	m_direction			= true;							// 右向きに設定
	m_matelialType		= NowMatelialType::Idle;		// マテリアルの種類を初期化
	m_chaseFlg			= false;						// 追尾フラグを初期化
	RunFlg				= false;						// ダッシュフラグを初期化
	IdleFlg				= true;							// アイドルフラグを初期化
	DamageFlg			= false;						// ダメージフラグを初期化
	m_shotFlg			= false;						// ショットフラグを初期化


	//アニメーション初期化
	m_animeInfo.start	= 0.0f;								//開始コマを0.0fで初期化
	m_animeInfo.end		= 5.0f;								//終了コマを0.0fで初期化
	m_animeInfo.count	= 0.0f;								//現在のコマ数カウントを0.0fで初期化
	m_animeInfo.speed	= 0.2f;								//アニメーション速度を0.0fで初期化

	m_enemyType		= EnemyType::Shot;							// 敵の種類を設定

	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();

	m_pCollider		= std::make_unique<KdCollider>();

	// 当たり判定の形状を設定
	m_pCollider->RegisterCollisionShape(
		"EnemyCollision",
		Math::Vector3(0, 0.5, 0),							// 球の中心座標（原点からの位置）
		m_attackarea,										// 球の半径
		KdCollider::TypeDamage								// 当たり判定のタイプ
	);

	m_objectType = ObjctType::Enemy;						// オブジェクトの種類を設定

	m_hp = 100;												// HPを設定

	DamageEndFlg = true;									// ダメージ終了フラグを初期化

	Alive = true;
}
void ShotEnemy::Update() 
{

	m_dir = {};

	NowMatelialType oldMatelialType = m_matelialType;

	//========================移動処理============================
	if(!DamageFlg)
	{
		{
			// ==========================================
			// 追尾対象が一定範囲にいるか？
			// ==========================================
			Math::Vector3 targetPos = {};
			// weak_ptrで所持している m_spTarget が有効かどうかを調べる
			// expired() … shared_ptrとして無効なアドレスならtrueを返す
			if (m_hp > 0/* && !DamageFlg*/)
			{
				if (m_target.expired() == false)
				{
					// lock() … weakをshared_ptrに変換
					targetPos = m_target.lock()->GetPos();
					// 対象との距離(ベクトルの長さ)で判定　※球判定
					Math::Vector3 v = targetPos - m_pos;
					// ベクトルの長さが一定値以下なら追尾モードへ

					if (v.Length() < m_searchArea && v.Length() > m_dashthArea)
					{
						m_chaseFlg = true;
						m_matelialType = NowMatelialType::Dash;
						AtkFlg = false;		// 攻撃フラグをリセット
					}
					else if (v.Length() < m_searchArea && v.Length() < m_dashthArea)
					{
						m_chaseFlg = false;
						m_matelialType = NowMatelialType::Atk1;
						AtkFlg = true;
					}
					else
					{
						m_chaseFlg = false;

						IdleFlg = true;										// 追尾対象がいないのでアイドル状態にする
						m_matelialType = NowMatelialType::Idle;				// マテリアルをアイドルに変
						AtkFlg = false;		// 攻撃フラグをリセット
					}
					//追尾フラグONの場合は追尾する
					if (m_chaseFlg)
					{
						// y軸を 0 にしておく　※空飛んで追いかけるの防止
						v.y = 0.0f;

						// 対象へのベクトルの長さを 1 に
						v.Normalize();

						// 方向を向く
						if (v.x < 0.0f)
						{
							m_direction = false;				// 左向き
						}
						else
						{
							m_direction = true;					// 右向き
						}

						// 方向確定
						m_dir = v;
					}
					else
					{
						m_dir = {};
					}

				}
			}
		}
		//============================================================
		if (!(m_matelialType == NowMatelialType::Damege) && !(m_matelialType == NowMatelialType::Death))
		{
			if (oldMatelialType != m_matelialType)
			{
				// マテリアルの種類が変わったらマテリアルを変更
				ChangeMatelialType();
			}
		}
	}
	////アニメーションの更新作業
	m_animeInfo.count += m_animeInfo.speed;    //コマ数を加算

	int animeCnt = static_cast<int>(m_animeInfo.start + m_animeInfo.count);

	if (AtkFlg && animeCnt == 3)
	{
		// 攻撃オブジェクトを出現させる座標を確定させる
		Math::Vector3 attackPos = {};
		attackPos = m_pos;			// プレイヤーの座標を基準にする	// 攻撃方向に0.4だけずらす
		// 攻撃オブジェクトを作成
		std::shared_ptr<EnemyAttack> attack;
		attack = std::make_shared<EnemyAttack>();
		attack->Init();									// 初期化
		attack->SetPos(attackPos);						// 座標をセット
		attack->SetDirection(m_direction);	// 攻撃方向をセット
		SceneManager::Instance().AddObject(attack);		// シーンに追加
		AtkFlg = false;		// 攻撃フラグをリセット
		
	}

	if (m_matelialType == NowMatelialType::Death && (animeCnt > m_animeInfo.end))
	{
		Alive = false;						// 死亡アニメーションが終わったらオブジェクトを削除
		m_isExpired = true;					// 死亡アニメーションが終わったらオブジェクトを削除
	}
	if (m_matelialType == NowMatelialType::Damege && (animeCnt > m_animeInfo.end))
	{
		DamageEndFlg = true;							// ダメージ終了フラグを立てる
		DamageFlg = false;								// ダメージフラグをリセット
	}

	// アニメーションのコマ数が終了コマを超えたら
	if (animeCnt > m_animeInfo.end)
	{
		// アニメーションのコマ数を開始コマに戻す
		m_animeInfo.count = 0.0f;
	}
	m_polygon->SetUVRect(animeCnt);

	m_pos.y -= m_gravity;
	m_gravity += 0.05f;
	m_pos += m_dir * m_speed;

	// 行列関係
	{
		int Rot = 0;
		if (!m_direction)
		{
			Rot = 180; //左向き
		}
		//拡縮行列
		Math::Matrix _mScale = Math::Matrix::CreateScale(1.0f);

		//回転行列
		Math::Matrix _mXRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
		Math::Matrix _mYRotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(Rot));
		Math::Matrix _mZRotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));

		//移動行列
		Math::Matrix _mTrans = Math::Matrix::CreateTranslation(m_pos);

		m_mWorld = _mScale * (_mXRotation * _mYRotation * _mZRotation) * _mTrans;
	}
	//// プレイヤー検知範囲判定
	//{
	//	m_pDebugWire->AddDebugSphere
	//	(
	//		m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
	//		m_searchArea,			// 球の半径
	//		kRedColor			// 球の色
	//	);
	//}
	//// プレイヤー検知範囲判定
	//{
	//	m_pDebugWire->AddDebugSphere
	//	(
	//		m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
	//		m_dashthArea,			// 球の半径
	//		kBlueColor			// 球の色
	//	);
	//}
	//// プレイヤー検知範囲判定
	//{
	//	m_pDebugWire->AddDebugSphere
	//	(
	//		m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
	//		m_ShotArea,			// 球の半径
	//		kGreenColor			// 球の色
	//	);
	//}
}
void ShotEnemy::PostUpdate()
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
	rayInfo.m_pos.y = 0.25f;
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
	}
	//========================
	//球判定
	//========================
	//球判定用変数
	KdCollider::SphereInfo spherInfo;
	//球の中心座標
	spherInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.75f, 0);
	//球の半径
	spherInfo.m_sphere.Radius = 0.55f;
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
void ShotEnemy::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}
void ShotEnemy::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_mWorld);
}

void ShotEnemy::ChangeMatelialType()
{

	if (m_matelialType == NowMatelialType::Idle)
	{
		m_polygon->SetMaterial(TexturePath::Idle);
		m_polygon->SetSplit(6,1);	// UVを0に設定
		m_animeInfo.end = 5;
		m_animeInfo.start = 0.0f;
		m_animeInfo.speed = 0.2f;
	}
	if (m_matelialType == NowMatelialType::Walk)
	{
		m_polygon->SetMaterial(TexturePath::Walk);
		m_polygon->SetSplit(8, 1);	// UVを0に設定
		m_animeInfo.end = 7;	// アニメーションの終了コマを6にする
		m_animeInfo.start = 0.0f;
		m_animeInfo.speed = 0.2f;
	}
	if (m_matelialType == NowMatelialType::Dash)
	{
		m_polygon->SetMaterial(TexturePath::Run);
		m_polygon->SetSplit(6, 1);	// UVを0に設定
		m_animeInfo.end = 5;	// アニメーションの終了コマを6にする
		m_animeInfo.start = 0.0f;
		m_animeInfo.speed = 0.2f;
	}
	if (m_matelialType == NowMatelialType::Atk1)
	{
		m_polygon->SetMaterial(TexturePath::Attack1);
		m_polygon->SetSplit(6, 1);	// UVを0に設定
		m_animeInfo.end = 5;	// アニメーションの終了コマを6にする
		m_animeInfo.start = 0.0f;
		m_animeInfo.speed = 0.2f;
	}
	m_animeInfo.count = 0.0f;
}

void ShotEnemy::SetExpired(bool flg)
{
	expired = flg;	// 有効期限フラグを設定a
}

void ShotEnemy::onHit()
{
	if(!DamageEndFlg)return;	// ダメージ終了フラグが立っている場合は何もしない

	m_hp -= 30;
	if (m_hp <= 0)
	{
		m_polygon->SetMaterial(TexturePath::Dead);					// ダメージを受けたらマテリアルを変更
		m_matelialType = NowMatelialType::Death;					// マテリアルの種類をDeadに変更
		m_polygon->SetSplit(4, 1);									// UVを0に設定
		m_animeInfo.end = 5;										// アニメーションの終了コマを6にする
		m_animeInfo.speed = 0.04f;
		m_animeInfo.start = 0.0f;
	}
	else
	{
		m_polygon->SetMaterial(TexturePath::Damege);				// ダメージを受けたらマテリアルを変更
		m_matelialType = NowMatelialType::Damege;					// マテリアルの種類をDamegeに変更
		m_polygon->SetSplit(2, 1);									// UVを0に設定
		m_animeInfo.speed								= 0.02f;	// アニメーションの速度を0.2fに設定
		m_animeInfo.end									= 1;		// アニメーションの終了コマを2にする
		DamageFlg										= true;		// ダメージフラグを立てる
		m_animeInfo.start								= 0;
	}
}
