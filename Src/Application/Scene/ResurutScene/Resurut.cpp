#include "Resurut.h"

void Result::Init()
{
	m_Tex.Load ("Asset/Textures/result.png");
	m_Tex2.Load("Asset/Textures/resultmoji.png");
}

void Result::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex, 0, 0);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex2, 0, 150);
}
