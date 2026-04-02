#include "I wanna be the engine suku edition.h"

using namespace suku;

class Room1 : public Room
{
public:
	virtual void onCreate()override
	{
		Room::onCreate();
		create<Player>(0, 0);
		create(Wall(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		//create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		//create(Spike(32, 128), Wall(32, 160));
	}
}room1;

class Room0 : public Room
{
public:

	virtual void onCreate()override
	{
		Room::onCreate();

		static SaveFile save0("save123");
		setSaveFile(&save0);

		//MCIDEVICEID a = openAudio("Audio\\musOnDeath.mp3");
		//playDevice(a, true);
		//music1.setVolume(0.01);
		//music1.setSpeed(0.5);
		//create(Background(Bitmap("Image\\bg.jpg")));

		//static Player player(0, 0);
		//append(&player);

		Wall a(100, 100);
		create(a);
		create<Player>(0, 0);
		//create(Wall(32, 32))->addDelayAction(100, 
		//	[&](Object* _this)->bool { _this->movingTo(64, 64, 50); return false;});
		create(Wall(32, 32))->addTimelineAction({ 100, 200 },
			{
				[](Object* _this)->bool { _this->movingTo(64, 64, 50); return false; },
				[](Object* _this)->bool { _this->movingTo(32, 32, 50); return false; }
			});
		create(Wall(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		create(Spike(32 + 16, 0, Direction::Down));
		create(Spike(32, 128), Wall(32, 160));
		// create(Warp(128, 0, &room1));
	}

	virtual void onPaintStart()override
	{
		auto brush = graphics::createSolidColorBrush(Color(255, 255, 255, 1.0f));
		auto brush2 = graphics::createSolidColorBrush(Color(0, 0, 0, 1.0f));
		auto brushBlack = graphics::createSolidColorBrush(Color(0, 0, 0, 1.0f));
		Shape A(SquareShape(64));
		Bitmap B("Image\\bg.jpg");
		static PaintLayer layer(800, 608);
		layer.beginDraw();
		layer.clear(Color::GREEN());
		layer.drawBitmap(B, translation(0, 0), 0.5f);
		layer.drawShape(A, translation(400, 300), brush, brush2, 5.0f);
		layer.endDraw().paint();
		Text a("Consolas", 24, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			TextAlign::MiddleRight);
		a.textContent = "test message";
		a.paint(256, 256, brushBlack);
	}
}room0;

void init()
{
	gotoRoom(room0);
}