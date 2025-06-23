#pragma once
class ShotEnemy : public KdGameObject
{
public:
	ShotEnemy(){}
	~ShotEnemy(){}

	enum NowMatelialType
	{
		Idle,
		Walk,
		Dash,
		Atk1,
		Atk2,
		Atk3,
		Jump,
		Death,
		Damege,
	};

	void Init() override;

	void Update() override;

	void PostUpdate() override;

	void DrawLit() override;

	void GenerateDepthMapFromLight() override;

	void SetTarget(std::shared_ptr<KdGameObject> target)
	{
		m_target = target;
	}

	void ChangeMatelialType();

	void SetExpired(bool flg)override;


	void onHit()override;

private:

	std::shared_ptr<KdSquarePolygon> m_polygon;

	std::weak_ptr<KdGameObject> m_target;

	Math::Vector3 m_pos			= {};
	Math::Vector3 m_move		= {};
	Math::Vector3 m_dir			= {};

	float m_gravity				= 0.0f;
	float m_searchArea			= 0.0f;
	float m_speed				= 0.0f;

	int m_hp					= 0;	// HP

	bool Left					= false;
	bool Right					= false;
	bool IdleFlg				= false;
	bool AtkFlg					= false;
	bool GuardFlg				= false;
	bool WalkFlg				= false;
	bool RunFlg					= false;
	bool JumpFlg				= false;
	bool KeyFlg					= false;
	bool m_direction			= false;
	bool m_chaseFlg				= false;

	bool expired				= false;	// 有効期限フラグ

	NowMatelialType		m_matelialType	= NowMatelialType::Idle;	// 現在のマテリアルタイプ

	AnimationInfo		m_animeInfo;
	float				m_animespeed	= 0.0f;		
};
