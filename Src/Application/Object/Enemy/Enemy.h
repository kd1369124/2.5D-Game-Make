#pragma once
class Enemy: public KdGameObject
{
public:
	Enemy(){}
	~Enemy(){}

	void Init() override;
	void DrawUnLit() override;
	void Update() override;
	void PreUpdate() override;

private:

	std::shared_ptr<KdSquarePolygon> m_pPolygon;

};
