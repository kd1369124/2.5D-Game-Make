#pragma once
class NomalBack : public KdGameObject
{
public:
	NomalBack(){}
	~NomalBack(){}

	void Init()override;

	void DrawUnLit()override;

private:

	//板ポリゴン
	KdSquarePolygon m_polygon;

};
