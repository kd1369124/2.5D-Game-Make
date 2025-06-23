#pragma once

class TestPlayer : public KdGameObject
{
public:



	TestPlayer() {}
	~TestPlayer() override {}

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;

	void GenerateDepthMapFromLight() override;

	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	Math::Vector3 GetPos() const { return m_pos; }

private:

	std::shared_ptr<KdSquarePolygon> m_polygon;
	//アニメーション
	AnimationInfo m_animeInfo;
	Math::Vector3 m_pos;
	Math::Vector3 m_move;

	//gravity
	float m_gravity;
	bool Left;
	bool Right;
	bool IdleFlg;
	bool AtkFlg;
	bool GuardFlg;
	bool WalkFlg;
	bool RunFlg;
	bool JumpFlg;
	bool KeyFlg;
};