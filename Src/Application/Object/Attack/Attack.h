#pragma once
class Attack : public KdGameObject
{
public:
	Attack(){}
	~Attack(){}

	void Init() override;
	void Update() override;
	void PostUpdate() override;

	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	void onHit()override;

	void SetDirection(bool direction)
	{
		m_direction = direction;
	}

private:

	Math::Vector3 m_pos = {};

	// 攻撃判定のエリア
	float m_attackArea = 0.0f;

	int FrameCount = 0;

	bool m_direction = true; // true: 右向き, false: 左向き

};

