#include "NomalBack.h"

void NomalBack::Init()
{
	m_polygon.SetMaterial("Asset/Textures/BackGround.png");
	m_polygon.SetScale(200.0f);
	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	//m_mWorld = Math::Matrix::CreateTranslation(0, 0, 100);
	m_mWorld = Math::Matrix::CreateTranslation
	(Math::Vector3(0, 0, 100));
}

void NomalBack::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}
