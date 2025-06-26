#pragma once
class Result : public KdGameObject
{
public:
	Result(){}
	~Result(){}

	void Init()override;

	void DrawSprite()override;


private:

	KdTexture m_Tex;
	KdTexture m_Tex2;

};
