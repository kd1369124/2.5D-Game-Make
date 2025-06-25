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

	Math::Vector3 m_dir;

	float m_speed = 0.0f;

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
	bool ZmovepulsFlg = NULL;
	bool ZKeyFlg;
	bool ZmoveminusFlg = NULL;	
	bool m_direction = true; // true:右, false:左

};