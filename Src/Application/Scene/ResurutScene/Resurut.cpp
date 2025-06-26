#include "Resurut.h"

void Result::Init()
{
	m_Tex.Load("Asset/Textures/result.png");
}

void Result::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex, 0, 0);
}
