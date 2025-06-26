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

	void onHit()override;

	void SetHp(int hp) { m_hp = std::clamp(hp, 0, m_maxHp); }
	int GetHp()const { return m_hp; }
	int GetMaxHp()const { return m_maxHp; }

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

	float m_HitArea = 0.0f; // 攻撃判定のエリア

	bool DamageFlg = false;	// ダメージフラグ

	bool DamageEndFlg = false;	// ダメージ終了フラグ

	bool FrashFlg = false;	// フラッシュフラグ

	int								m_hp = 500;
	int								m_maxHp = 500;

	int FlashTimer = 0;         // 点滅用フレームカウンター
	bool Visible = true;        // 現在表示状態か

};