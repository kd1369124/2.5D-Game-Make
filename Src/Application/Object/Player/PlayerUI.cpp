#include "PlayerUI.h"
#include"TestPlayer.h"

void PlayerUI::Init()
{

	m_tex = std::make_shared<KdTexture>();
	m_tex2 = std::make_shared<KdTexture>();
	m_tex3 = std::make_shared<KdTexture>();
	
	m_tex->Load ("Asset/Textures/waku.png");
	m_tex2->Load("Asset/Textures/HPbarGray.png");
	m_tex3->Load("Asset/Textures/HPbarGreen.png");

	
}

//↓位置もHp共有とかないから　タイトルとかで使える
//void PlayerUI::DrawSprite()
//{
//	const float scaleX = 300.0f; 
//	const float scaleY = 50.0f; 
//	KdShaderManager::Instance().m_spriteShader.DrawTex(&*m_tex, -400, 300, scaleX, scaleY);
//}

void PlayerUI::DrawSprite()
{
	auto playerPtr = m_player.lock();
	if (!playerPtr) return;

	float maxHp = static_cast<float>(playerPtr->GetMaxHp());
	m_displayHp = static_cast<float>(playerPtr->GetHp());
	float hpRate = std::clamp(m_displayHp / maxHp, 0.0f, 1.0f);

	const float barWidth = 300.0f;
	const float barHeight = 50.0f;
	float posX = -550;
	float posY = -300;

	// 背景を描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(&*m_tex2, posX + barWidth / 2, posY, barWidth, barHeight);

	// HP本体（赤or緑）
	Math::Color barColor = (playerPtr->GetHp() <= 3) ? Math::Color(1, 0, 0, 1) : Math::Color(0, 1, 0, 1);
	float hpFillWidth = barWidth* hpRate;

	KdShaderManager::Instance().m_spriteShader.DrawTex(*&m_tex3, posX + hpFillWidth / 2, posY, hpFillWidth, barHeight, nullptr/*, &barColor */ );

	// 枠を描画（常に最前面）
	KdShaderManager::Instance().m_spriteShader.DrawTex(&*m_tex, posX + barWidth / 2, posY, barWidth, barHeight);
}


void PlayerUI::Update()
{

	auto player = m_player.lock();
	if (!player) return;

	float targetHp = static_cast<float>(player->GetHp());

	const float followSpeed = 0.05f;  // 追従速度(右に寄せる)

	m_displayHp += (targetHp - m_displayHp) * followSpeed;

	if (std::abs(targetHp - m_displayHp) < 0.02f)
	{
		m_displayHp = targetHp;
	}
}