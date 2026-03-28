#include "I wanna be the engine suku edition.h"

using namespace suku;

class Room1 : public Room
{
public:
	virtual void onCreate()override
	{
		Room::onCreate();
		playerStartX = 0.0;
		playerStartY = 0.0;
		create(Player(0, 0));
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
		Room::onCreate();

		File a("save0.sav");
		a.create();

		playerStartX = 0.0;
		playerStartY = 0.0;

		//MCIDEVICEID a = openAudio("Audio\\musOnDeath.mp3");
		//playDevice(a, true);
		//music1.setVolume(0.01);
		//music1.setSpeed(0.5);
		//create(Background(Bitmap("Image\\bg.jpg")));
		create(Player(0, 0));
		create(Wall(32, 32))->setDelayAction(100, [&](Object* _this) { _this->movingTo(64, 64, 50); });
		create(Wall(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		create(Wall(96, 64), Wall(64, 96), Wall(96, 96), Wall(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		create(Spike(32 + 16, 0, Direction::Down));
		create(Spike(32, 128), Wall(32, 160));
		// create(Warp(128, 0, &room1));
		createPath("a\\b\\c");
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