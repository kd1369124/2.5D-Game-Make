#include"Attack.h"
#include"../Player/TestPlayer.h"
#include"../../Scene/SceneManager.h"
void Attack::Init()
{
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Attack::Update()
{
	//========================
	//球判定
	//========================
	//球判定用変数
	KdCollider::SphereInfo spherInfo;
	//球の中心座標
	spherInfo.m_sphere.Center = m_pos;
	//球の半径
	spherInfo.m_sphere.Radius = 0.3f;
	//当たり判定したいタイプを設定
	spherInfo.m_type = KdCollider::TypeDamage;

	//デバック
	m_pDebugWire->AddDebugSphere(spherInfo.m_sphere.Center, spherInfo.m_sphere.Radius);


	//球と当たり判定をする!!
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(spherInfo, nullptr) == true)
		{
			if ((obj->GetEnemyType()) != EnemyType::NullEnemy )
			{
				if ((obj->GetEnemyType()) == EnemyType::Nomal)
				{
					obj->onHit(); //当たり判定があったらダメージを与える

				}
				if ((obj->GetEnemyType()) == EnemyType::Shot)
				{
					obj->onHit(); //当たり判定があったらダメージを与える
				}
				if ((obj->GetEnemyType()) == EnemyType::Shotsamurai)
				{
					obj->onHit(); //当たり判定があったらダメージを与える

				}
			}
		}
	}
}

void Attack::PostUpdate()
{

}
