#pragma once

#include"../BaseScene/BaseScene.h"
class Player;
class Ground;
class NomalBack;
class TestPlayer;
class KdGameObject;
class GameScene : public BaseScene
{
public :

	GameScene()  { Init(); }
	~GameScene() {}

private:

	void Event() override;
	void Init()  override;

	bool InitFlg = false;

	std::shared_ptr<Player>player;
	std::shared_ptr<TestPlayer>testplayer;
	std::shared_ptr<Ground>ground;
	std::shared_ptr<NomalBack>nBack;

};
