#include "I wanna be the engine suku edition.h"

using namespace suku;

Room room0;

void init()
{
	room0.create(Player(0, 0))->rotate(50.0f);
	room0.create(Wall(32, 32))->setDelayAction(100, [](Object* _this) {_this->movingTo(64, 64, 50); });
	room0.create(Wall(96, 32));
	room0.create(VineLeft(96, 32));
	room0.create(VineRight(96, 32));
	room0.create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
	room0.create(WaterExtraJump(160, 32));
	room0.create(Water(224, 32));
	room0.create(Spike(32, 128), Wall(32, 160));
	linkCollisionType<Wall, Spike>();
	nowRoom = &room0;
}