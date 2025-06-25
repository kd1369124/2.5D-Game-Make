#include "Ground.h"

void Ground::Init()
{
	{
		m_model = std::make_shared<KdModelData>();
		m_model->Load("Asset/Models/Test.gltf");
		m_GroundScale = { 1.0f,1.0f,0.4f };
		//行列
		Math::Matrix _Scale =
			
			Math::Matrix::CreateScale(m_GroundScale);
		Math::Matrix _Trans =
			Math::Matrix::CreateTranslation(0, -1.0f, 0);

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
