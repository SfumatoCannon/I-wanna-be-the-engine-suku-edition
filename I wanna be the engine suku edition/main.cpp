#include "I wanna be the engine suku edition.h"

using namespace suku;

class Room1 : public Room
{
public:
	virtual void onCreate()override
	{
		objectPointerArray.clear();
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
		objectPointerArray.clear();
		playerStartX = 0.0;
		playerStartY = 0.0;
		create(Background(Bitmap("Image\\bg.jpg")));
		create(Player(0, 0))->rotate(50.0f);
		create(Wall(32, 32))->setDelayAction(100, [](Object* _this) {_this->movingTo(64, 64, 50); });
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
		Shape A(SquareShape(64));
		Shape B = A;
		A = CircleShape(32);
		static ID2D1Brush* brush1 = createSolidColorBrush(Color(255, 0, 0));
		static ID2D1Brush* brush2 = createSolidColorBrush(Color(125, 125, 0, 0.4f));
		B.setTransform(rotation(16, 16, 30));
		B.paint(32, 64, brush1, brush1);
		A.paint(32, 128, brush2, brush1);
	}
}room0;

void init()
{
	gotoRoom(room0);
}