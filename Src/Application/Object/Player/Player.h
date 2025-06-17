#pragma once
class Player : public KdGameObject
{
public:
	Player(){}
	~Player(){}

	void Init()override;

	void Update()override;
	void PostUpdate()override;

	void GenerateDepthMapFromLight() override;
	void DrawUnLit()override;

	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	Math::Vector3 GetPos() const { return m_pos; }

	


private:

	KdSquarePolygon m_polygon;

	Math::Vector3 m_pos;

	Math::Vector3 m_dir;

	float m_speed = 0.0f;

	float m_anime = 0;

	float m_gravity = 0;

};
