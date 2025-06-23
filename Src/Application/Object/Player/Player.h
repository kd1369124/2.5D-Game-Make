#pragma once
enum NowMatelialType
{
	Idle,
	Walk,
	Dash,
	Atk1,
	Atk2,
	Atk3,
	Jump,
	Death

};
enum SlideType 
{
	Walkslide = 8,
	Dashslide = 8,
	Idleslide = 6,
	Jumpslide = 12,
	Hurtslide = 2,
	Atk1slide = 6,
	Atk2slide = 4,
	Atk3slide = 3,
	Deathslide = 3,


};

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

	void MatelialType(bool walk = false, bool dash = false, bool atk1 = false , bool atk2 = false , bool atk3 = false , bool jump = false);

	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	Math::Vector3 GetPos() const { return m_pos; }

	


private:

	KdSquarePolygon m_polygon;

	NowMatelialType m_matelialType;



	Math::Vector3 m_pos;

	Math::Vector3 m_dir;

	float m_speed = 0.0f;

	float m_anime = 0;

	float m_gravity = 0;

	//======================行動フラグ======================
	bool m_WalkkeyFlg = false;

	bool m_JumpkeyFlg = false;

	bool m_AttackkeyFlg = false;

	bool m_AttackkeyFlg2 = false;

	bool m_AttackkeyFlg3 = false;

	bool m_DashkeyFlg = false;

	bool m_DeathFlg = false;

	bool m_direction = true; // true:右, false:左
	//====================================================
};
