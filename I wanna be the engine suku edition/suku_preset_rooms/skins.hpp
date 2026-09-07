#pragma once

#include <I wanna be the engine suku edition.h>

using namespace suku;

class RoomStage0 : public Room
{
public:
	inline static TilePack tilepack{
		Tile{ Block::spr, Sprite("Image\\stage0\\block.png", 2, 1, 0, SquareShape(32)) },
		Tile{ Spike::sprUp, Sprite("Image\\stage0\\spike_u.png") },
		Tile{ Spike::sprDown, Sprite("Image\\stage0\\spike_d.png") },
		Tile{ Spike::sprLeft, Sprite("Image\\stage0\\spike_l.png") },
		Tile{ Spike::sprRight, Sprite("Image\\stage0\\spike_r.png") }
	};
	RoomStage0(unsigned int _width = 800, unsigned int _height = 600) : Room(_width, _height)
	{
		setBackground(Color(200, 180, 180));
		tilepack.use();
	}
};