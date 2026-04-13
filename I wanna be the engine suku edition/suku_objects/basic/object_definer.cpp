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