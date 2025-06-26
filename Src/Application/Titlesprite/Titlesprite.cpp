#include"Titlesprite.h"

void Title::Init()
{
	m_Tex.Load("Asset/Textures/raw.png");
	m_Tex2.Load("Asset/Textures/aa.png");
	m_Tex3.Load("Asset/Textures/titlemoji.png");
}

void Title::DrawSprite()
{

	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex, 0, 0);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex2, 0, 150);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex3, 0, -150);

}
