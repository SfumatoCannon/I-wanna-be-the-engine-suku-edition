#include "object_definer.h"
#include "suku_sounds.h"

using namespace suku::maths;

namespace suku
{

	Sprite Spike::sprUp(BitmapSpriteElement("Image\\spike_u.png", 16, 16));
	Sprite Spike::sprDown(BitmapSpriteElement("Image\\spike_d.png", 16, 16));
	Sprite Spike::sprLeft(BitmapSpriteElement("Image\\spike_l.png", 16, 16));
	Sprite Spike::sprRight(BitmapSpriteElement("Image\\spike_r.png", 16, 16));
	Spike::Spike(float _x, float _y, Direction _dir) :Object(_x, _y)
	{
		setPaintId(3);
		switch (_dir)
		{
		case Direction::Up:
			sprite_ = &sprUp;
			break;
		case Direction::Down:
			sprite_ = &sprDown;
			break;
		case Direction::Left:
			sprite_ = &sprLeft;
			break;
		case Direction::Right:
			sprite_ = &sprRight;
			break;
		default:
			sprite_ = &sprUp;
			break;
		}
	}

	Sprite Blood::spr(BitmapSpriteElement("Image\\blood.png", SquareShape(2)));
	Blood::Blood(float _x, float _y, float _wspeed, float _hspeed) :Object(_x, _y)
	{
		setPaintId(4);
		hspeed = _wspeed;
		vspeed = _hspeed;
		sprite_ = &spr;
		gravity = 0.2f;
	}

	void Blood::preUpdate()
	{
		if (input::isKeyDown(input::VK_R))
			destroy();
		if (vspeed == 0 && hspeed == 0)
			return;
		vspeed += gravity;
	}

	void Blood::update()
	{
		auto tempList = getCrashedObjectList<Wall>(x + totalHspeed(), y + totalVspeed(), true);

		float vspeedBefore = totalVspeed();

		for (auto& wall : tempList)
			moveContactOld(*wall);

		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Wall>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;
	}

	Background::Background(const Bitmap& _bitmap) : picture(_bitmap), Object(0, 0)
	{
		setPaintId(0);
	}

	Background::Background(const Background& _bg) : picture(_bg.picture), Object(0, 0)
	{
		setPaintId(0);
	}

	Background::Background(Background&& _bg) noexcept : picture(std::move(_bg.picture)), Object(0, 0)
	{
		setPaintId(0);
	}
	//Background::Background(const Background& _bg)
	//{
	//	if (picture)
	//	{
	//		delete picture;
	//		picture = nullptr;
	//	}
	//	picture = new Bitmap(*_bg.picture);
	//}

	bool Background::onPaint()
	{
		picture.paint(0, 0);
		return true;
	}

}