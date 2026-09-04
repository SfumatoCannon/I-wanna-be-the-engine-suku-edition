#include "I wanna be the engine suku edition.h"
#include "suku_preset_rooms/includes.h"
using namespace suku;

class Room1 : public Room
{
public:
	Room1() : Room()
	{
		create(Block(96, 32));
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		//create(Block(96, 64), Block(64, 96), Block(96, 96), Block(128, 96));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		//create(Spike(32, 128), Block(32, 160));
	}
};

class Room0 : public Room
{
public:
	Room0() : Room(2000, 1216)
	{
		setBackground("Image\\bg.jpg");

		static TilePack a(
			Tile{ Block::spr, Sprite("Image\\stage0\\block.png", 2, 1, 0, SquareShape(32)) },
			Tile{ Spike::sprUp, Sprite("Image\\stage0\\spike_u.png") },
			Tile{ Spike::sprDown, Sprite("Image\\stage0\\spike_d.png") },
			Tile{ Spike::sprLeft, Sprite("Image\\stage0\\spike_l.png") },
			Tile{ Spike::sprRight, Sprite("Image\\stage0\\spike_r.png") }
		);
		a.use();

		static SaveFile save0("save123");
		setGlobalSaveFile(&save0);

		create<Block>(1400, 400);
		Player* p = create<Player>(0.f, 0.f).get();
		int x = 0;
		setSavable<int>("test");
		if (!getGlobalSaveFile()->hasValue("test"))
		{
			x = 0;
		}
		else
			x = getGlobalSaveFile()->loadVar<int>("test");
		x++;
		getGlobalSaveFile()->saveVar("test", x);

		create(Block(32, 32))->addTimelineAction({ 100, 200 },
			{
				[=](Object* _this)->bool { _this->movingTo(64 + x * 32, 64 + x * 32, 50); return false; },
				[](Object* _this)->bool { _this->movingTo(32, 32, 50); return false; }
			}
		);
		create(Block(96, 32))->addAction([=](Object* _this) {
			if (input::Mouse::isDoubleClick())
			{
				_this->x = input::Mouse::getX();
				_this->y = input::Mouse::getY();
			}
			return true;
			}
		);
		create(Block(256, 256))->addAction([=](Object* _this) {
			if (input::isKeyHolding(input::VK_A))
			{
				_this->rotate(14);
			}
			if (input::isKeyHolding(input::VK_D))
			{
				_this->rotate(-14);
			}
			return true;
			}
		);
		create(VineLeft(96, 32));
		create(VineRight(96, 32));
		create(Block(96, 64), Block(64, 96), Block(96, 96), Block(128, 96));
		create(Block(800, 32));
		for (int i = 64; i < 608; i += 32)
			create(Block(300, i));
		create(WaterExtraJump(160, 32));
		create(Water(224, 32));
		create(Spike(32 + 16, 0, Direction::Down));
		create(Spike(32, 128), Block(32, 160));
		create<Warp<Room1>>(128, 0);
		create<PausePage>();
		// setBGM(&suku::SoundPool::musGuyRock);
		//camera.angle = {720, Transition(100.0, TransitionCurve::easeOutExpo)};
		camera.setBind(Camera::follow(p, 0.2));
	}

	virtual void onUpdateStart() override
	{
	}

	virtual void onPaintStart(PaintLayer& _layer)override
	{
		backgroundOffsetX += 0.2;
		backgroundOffsetY += 0.5;

		Room::onPaintStart(_layer);
		
		//auto brush = graphics::createSolidColorBrush(Color(255, 255, 255, 1.0f));
		//auto brush2 = graphics::createSolidColorBrush(Color(0, 0, 0, 1.0f));
		auto brushBlack = graphics::createSolidColorBrush(Color(0, 0, 0, 1.0f));
		//Shape A(SquareShape(64));
		//Bitmap B("Image\\bg.jpg");
		//Bitmap C("Image\\wall.png");
		//static PaintLayer layer(1000, 608);
		//layer.beginDraw();
		//layer.clear(Color::GREEN());
		//layer.drawBitmap(B, translation(0, 0), 0.5f);
		//layer.drawShape(A, translation(400, 300), brush, brush2, 5.0f);
		//layer.drawBitmap(C, 128, 256, 0, 0, 16, 16);
		//layer.endDraw().paint();
		Text a("Consolas", 24, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			TextAlign::MiddleRight);
		a.contentString = std::to_wstring(getObjectList<Player>().front()->y.getValue());
		//a.contentString = "test message\npress s to save";
		a.paint(256, 256, brushBlack);
	}
};

void init()
{
	gotoRoom<Room0>();
	//gotoRoom<RoomTitle>();
}