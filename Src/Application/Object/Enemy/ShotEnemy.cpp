#include "ShotEnemy.h"
#include"../../Scene/SceneManager.h"
#include"../../Object/Player/TestPlayer.h"

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
	m_polygon->SetScale(2);											// ポリゴンのスケールを設定
	m_polygon->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);	// ポリゴンのピボットを設定

	m_pos = {};							// 初期位置を設定
	m_gravity = 0.5f;					// 重力の値を設定
	m_mWorld = Math::Matrix::Identity;	// ワールド行列を単位行列で初期化
	m_searchArea = 4.0f;					// プレイヤー検知範囲の半径を設定
	m_dashthArea = 2.0f;				// ダッシュ攻撃範囲の半径を設定
	m_attackarea = 0.7f;				// 攻撃範囲の半径を設定
	m_speed = 0.3f;					// 移動速度を設定
	m_direction = true;				// 右向きに設定
	m_matelialType = NowMatelialType::Idle;	// マテリアルの種類を初期化
	m_chaseFlg = false;				// 追尾フラグを初期化
	RunFlg = false;				// ダッシュフラグを初期化
	IdleFlg = true;				// アイドルフラグを初期化


	//アニメーション初期化
	m_animeInfo.start	= 0.0f;	//開始コマを0.0fで初期化
	m_animeInfo.end		= 0.0f;		//終了コマを0.0fで初期化
	m_animeInfo.count	= 0.0f;	//現在のコマ数カウントを0.0fで初期化
	m_animeInfo.speed	= 0.0f;	//アニメーション速度を0.0fで初期化

	m_enemyType = EnemyType::Shot;	// 敵の種類を設定

	m_pCollider = std::make_unique<KdCollider>();
	// 当たり判定の形状を設定
	m_pCollider->RegisterCollisionShape(
		"ShotEnemyCollision",			// 当たり判定の名称
		m_polygon,
		KdCollider::TypeDamage);		// 当たり判定の種類
}
void ShotEnemy::Update()
{

	m_dir = { 0,0,0 };

	NowMatelialType oldMatelialType = m_matelialType;



	// ==========================================
	// 追尾対象が一定範囲にいるか？
	// ==========================================
	Math::Vector3 targetPos = {};
	// weak_ptrで所持している m_spTarget が有効かどうかを調べる
	// expired() … shared_ptrとして無効なアドレスならtrueを返す
	if (m_target.expired() == false)
	{
		// lock() … weakをshared_ptrに変換
		targetPos = m_target.lock()->GetPos();
		// 対象との距離(ベクトルの長さ)で判定　※球判定
		Math::Vector3 v = targetPos - m_pos;
		// ベクトルの長さが一定値以下なら追尾モードへ
		if (v.Length() < m_searchArea)
		{
			if (v.Length() < m_dashthArea)
			{
				RunFlg = true; // ダッシュフラグを立てる
			}
			m_chaseFlg = true;
		}
		else
		{
			m_chaseFlg = false;
			IdleFlg = true; // 追尾対象がいないのでアイドル状態にする
		}
		// 追尾フラグONの場合は追尾する
		if (m_chaseFlg)
		{
			// y軸を 0 にしておく　※空飛んで追いかけるの防止
			v.y = 0.0f;
			// 対象との距離で正規化前に攻撃か移動のみか射撃か判別する
			if ((v.x < m_attackarea) && RunFlg )
			{
				AtkFlg = true; // 攻撃フラグを立てる
			}

			// 対象へのベクトルの長さを 1 に
			v.Normalize();

			// 方向を向く
			if (v.x < 0.0f)
			{
				m_direction = false; // 左向き
			}
			else
			{
				m_direction = true; // 右向き
			}

			// 方向確定
			m_dir = v;
		}
		else
		{
			m_dir = {};
		}
	}

	if (oldMatelialType != m_matelialType)
	{
		m_animeInfo.count = 0.0f; //アニメーションをリセット
		ChangeMatelialType();
	}
	m_animeInfo.count += m_animeInfo.speed;    //コマ数を加算

	int animeCnt = static_cast<int>(m_animeInfo.start + m_animeInfo.count);

	if(animeCnt > m_animeInfo.end)
	{
		if ((animeCnt > m_animeInfo.end) && m_matelialType == NowMatelialType::Death)
		{
			m_isExpired = true; // 死亡アニメーションが終了したらオブジェクトを削除
		}
		animeCnt = m_animeInfo.start; //コマ数が終了コマを超えたら開始コマに戻す
	}

	m_polygon->SetUVRect(animeCnt);
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
	// プレイヤー検知範囲判定
	{
		m_pDebugWire->AddDebugSphere
		(
			m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
			m_searchArea,			// 球の半径
			kRedColor			// 球の色
		);
	}
	// プレイヤー検知範囲判定
	{
		m_pDebugWire->AddDebugSphere
		(
			m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
			m_dashthArea,			// 球の半径
			kBlueColor			// 球の色
		);
	}
	// プレイヤー検知範囲判定
	{
		m_pDebugWire->AddDebugSphere
		(
			m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
			m_attackarea,			// 球の半径
			kGreenColor			// 球の色
		);
	}

}
void ShotEnemy::PostUpdate()
{

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
		m_polygon->SetUVRect(6,1);	// UVを0に設定
		m_animeInfo.end = 5;
	}
	if (m_matelialType == NowMatelialType::Walk)
	{
		m_polygon->SetMaterial(TexturePath::Walk);
		m_polygon->SetUVRect(8, 1);	// UVを0に設定
		m_animeInfo.end = 7;	// アニメーションの終了コマを6にする
	}

}

void ShotEnemy::SetExpired(bool flg)
{
	expired = flg;	// 有効期限フラグを設定
}

void ShotEnemy::onHit()
{
	if (m_hp <= 0)
	{
		m_polygon->SetMaterial(TexturePath::Dead);		// ダメージを受けたらマテリアルを変更
		m_matelialType = NowMatelialType::Death;		// マテリアルの種類をDeadに変更
	}
	else
	{
		m_polygon->SetMaterial(TexturePath::Damege);	// ダメージを受けたらマテリアルを変更
		m_matelialType = NowMatelialType::Damege;		// マテリアルの種類をDamegeに変更
		m_animeInfo.end = 1;							// アニメーションの終了コマを2にする
		m_hp -= 30;
	}
}
