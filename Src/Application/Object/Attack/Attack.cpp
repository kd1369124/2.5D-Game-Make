﻿#include"Attack.h"
#include"../Player/TestPlayer.h"
#include"../../Scene/SceneManager.h"
void Attack::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	m_bgm = std::shared_ptr<KdSoundInstance>(); // BGMの初期化
	// 攻撃判定のエリア
	m_attackArea = 0.3f;
	// 単位行列
	m_mWorld = Math::Matrix::Identity;

	FrameCount = 0;

	m_direction = true; // true:右方向, false:左方向
}

void Attack::Update()
{
	FrameCount++;
	if (FrameCount >= 10)
	{
		m_isExpired = true; // 5フレーム経過したら消える
	}
	// ============================================================
	// 当たり判定　球判定　ここから
	// ============================================================

	// 球判定用の変数作成
	KdCollider::SphereInfo sphereInfo;

	float posX = 0;
	
	if (m_direction)
	{
		posX =  0.6f ;
	}
	if(!m_direction)
	{
		posX =  -0.6f ;
	}

	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = m_pos+ Math::Vector3(posX, 0.75f,0);

	// 球の半径を設定
	sphereInfo.m_sphere.Radius = m_attackArea;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ用
	/*m_pDebugWire->AddDebugSphere
	(
		sphereInfo.m_sphere.Center,
		sphereInfo.m_sphere.Radius,
		kRedColor
	);*/

	// 球情報と当たり判定

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// 戻り値　当たっていたら true
		if (obj->Intersects(sphereInfo, nullptr) && obj->GetObjectType() == KdGameObject::ObjctType::Enemy)
		{
			//当たった
			obj->onHit();         // 敵が消える
			if (!SoundFlg)
			{
				m_bgm = KdAudioManager::Instance().Play("Asset/Sounds/AttackSound10.wav", false); 
				if (m_bgm)
				{
					m_bgm->SetVolume(0.2f);
				}
				SoundFlg = true; // 音を鳴らしたフラグを立てる
			}
			m_isExpired = true; // 攻撃が当たったら消える
		} 
		if (obj->Intersects(sphereInfo, nullptr) == false && obj->GetObjectType() == KdGameObject::ObjctType::Enemy)
		{
			if (!SoundFlg)
			{
				m_bgm = KdAudioManager::Instance().Play("Asset/Sounds/AttackSound11.wav", false); // 攻撃が当たった音を鳴らす
				if (m_bgm)
				{
					m_bgm->SetVolume(0.2f);
				}
				SoundFlg = true; // 音を鳴らしたフラグを立てる
			}
		}
	}
	//=============================================================
	// 当たり判定  球判定　ここまで
	//=============================================================
}

void Attack::PostUpdate()
{

}

void Attack::onHit()
{
	m_isExpired = true; // 攻撃が当たったら消える
}
