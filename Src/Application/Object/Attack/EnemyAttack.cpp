#include "EnemyAttack.h"
#include"../../Scene/SceneManager.h"


void EnemyAttack::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	// 攻撃判定のエリア
	m_attackArea = 0.3f;
	// 単位行列
	m_mWorld = Math::Matrix::Identity;

	FrameCount = 0;

	m_direction = true; // true:右方向, false:左方向
}

void EnemyAttack::Update()
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
		posX = 0.6f;
	}
	if (!m_direction)
	{
		posX = -0.6f;
	}

	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(posX, 0.75f, 0);

	// 球の半径を設定
	sphereInfo.m_sphere.Radius = m_attackArea;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ用
	//m_pDebugWire->AddDebugSphere
	//(
	//	sphereInfo.m_sphere.Center,
	//	sphereInfo.m_sphere.Radius,
	//	kRedColor
	//);

	// 球情報と当たり判定

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// 戻り値　当たっていたら true
		if ((obj->Intersects(sphereInfo, nullptr)) &&obj->GetObjectType() == KdGameObject::ObjctType::Player)
		{
			//当たった

			obj->onHit();         // 敵が消える
			m_isExpired = true; // 攻撃が当たったら消える
		}
	}
	//=============================================================
	// 当たり判定  球判定　ここまで
	//=============================================================
}

void EnemyAttack::onHit()
{
	m_isExpired = true; // 攻撃が当たったら消える
}
