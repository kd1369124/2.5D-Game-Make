#include "Ground.h"

void Ground::Init()
{
	{
		m_model = std::make_shared<KdModelData>();
		m_model->Load("Asset/Models/ground_block.glb");
		//行列
		Math::Matrix _Scale =
			
			Math::Matrix::CreateScale(1.4, 0.5, 1.0);
		Math::Matrix _Trans =
			Math::Matrix::CreateTranslation(-10, -10, 40);
		m_mWorld = _Scale * _Trans;
	}

	m_pCollider = std::make_unique<KdCollider>();

	//当たり判定の形状を設定
	m_pCollider->RegisterCollisionShape(
		"GroundCollision",			//当たり判定の名称
		m_model,					//モデルの形状をセット
		KdCollider::TypeGround);	//当たり判定の種類

}

void Ground::Update()
{
}

void Ground::PostUpdate()
{
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
