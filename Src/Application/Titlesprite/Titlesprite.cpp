#include"Titlesprite.h"

void Title::Init()
{
	m_Tex.Load("Asset/Textures/TitleClick.png");
}

void Title::DrawSprite()
{

	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_Tex, 0, 0);

}
