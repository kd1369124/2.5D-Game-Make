#pragma once

class TestPlayer;

class PlayerUI : public KdGameObject
{
public:
	PlayerUI() {};
	~PlayerUI() {};

	void Init() override;
	void DrawSprite() override;
	void Update() override;

	void SetPlayer(std::weak_ptr<TestPlayer> player) { m_player = player; }

private:

	float m_displayHp = 0.0f;  // 表示用HP（追従して変化）
	std::weak_ptr<TestPlayer> m_player;
	std::shared_ptr<KdTexture>			m_tex = nullptr;
	std::shared_ptr<KdTexture>			m_tex2 = nullptr;
	std::shared_ptr<KdTexture>			m_tex3 = nullptr;
};
