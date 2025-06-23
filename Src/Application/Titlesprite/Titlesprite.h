#pragma once
class Title : public KdGameObject
{
public:

	Title(){}
	~Title(){}

	void Init()override;

	void DrawSprite()override;



private:

	KdTexture m_Tex;


};

