#pragma once
class Ground : public KdGameObject
{
public:
	Ground(){}
	~Ground(){}

	void Init()override;

	void Update()override;
	void PostUpdate()override;

	void DrawLit()override;

	void SetPos(const Math::Vector3& pos)
	{
		m_pos = pos;
	}

	Math::Vector3 GetScale() const
	{
		return m_GroundScale;
	}
private:

	Math::Vector3 m_GroundScale;

	std::shared_ptr<KdModelData> m_model;

	Math::Vector3 m_pos = Math::Vector3::Zero;

};