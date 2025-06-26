#pragma once
class EnemyAttack : public KdGameObject
{
public:
	EnemyAttack(){}
	~EnemyAttack(){}

	void Init() override;
	
	void Update() override;

	void onHit() override;
	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	void SetDirection(bool direction, bool directionZ = NULL)
	{
		m_direction = direction;
		m_directionZ = directionZ;
	}

private:

	Math::Vector3 m_pos = {};

	// 攻撃判定のエリア
	float m_attackArea = 0.0f;

	int FrameCount = 0;

	bool m_direction = true;	// true: 右向き, false: 左向き
	bool m_directionZ = true;	// true: 奥向き, false: 手前向き


};
