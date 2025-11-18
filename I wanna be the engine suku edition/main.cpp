#include "I wanna be the engine suku edition.h"

using namespace suku;

class Room1 : public Room
{
public:
	virtual void onCreate()override
	{
		playerStartX = 0.0;
		playerStartY = 0.0;
		create(Player(0, 0))->rotate(50.0f);
		create(Wall(32, 32))->setDelayAction(100, [](Object* _this) {_this->movingTo(64, 64, 50); });
		create(Wall(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		create(Spike(32, 128), Wall(32, 160));
	}
}room1;

class Room0 : public Room
{
public:

	virtual void onCreate()override
	{
		playerStartX = 0.0;
		playerStartY = 0.0;

		//MCIDEVICEID a = openAudio("Audio\\musOnDeath.mp3");
		//playDevice(a, true);
		//music1.setVolume(0.01);
		//music1.setSpeed(0.5);
		//create(Background(Bitmap("Image\\bg.jpg")));
		create(Player(0, 0))->rotate(50.0f);
		create(Wall(32, 32))->setDelayAction(100, [&](Object* _this) { _this->movingTo(64, 64, 50); });
		create(Wall(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		create(Spike(32, 128), Wall(32, 160));
		create(Warp(128, 0, &room1));
	}

	virtual void onPaintStart()override
	{
		ID2D1Brush* brush = createSolidColorBrush(Color(255, 255, 255, 255));
		ID2D1Brush* brush2 = createSolidColorBrush(Color(0, 0, 0, 255));
		Shape A(SquareShape(4));
		Bitmap B("Image\\bg.jpg");
		Bitmap* C = A.paintOnBitmap(B, 400, 300, brush, brush2);
		C->paint(0, 0);
		delete C;
	}
}room0;

void init()
{
	gotoRoom(room0);
}