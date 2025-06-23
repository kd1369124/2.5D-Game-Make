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

	void SetExpired();

private:

	Math::Vector3 m_pos = {};

};

