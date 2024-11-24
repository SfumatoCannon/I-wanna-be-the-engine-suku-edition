#pragma once
#include "basis.h"

namespace suku
{
	__declspec(selectany) Sprite
		sprWall, sprSpikeUp, sprSpikeDown, sprSpikeLeft, sprSpikeRight,
		sprWater, sprWaterExtraJump, sprBlood,
		sprTrigger, sprWarp, sprCherry, sprPlatform, sprVineLeft, sprVineRight,
		sprPlayerStanding, sprPlayerRunning, sprPlayerFalling, sprPlayerJumping, sprPlayerSliding,
		sprBox
		;

	inline void spritePreSet()
	{
		sprTrigger.push(ShapeSpriteZero(SquareShape(32)));

		sprBlood.push(BitmapSpriteZero(2, 2, SquareShape(2), 1, 1, L"Image\\blood.png"));

		sprWarp.push(BitmapSpriteZero(L"Image\\warp.png", 16, 16));

		sprPlatform.push(BitmapSpriteZero(32, 16, RectangleShape(32, 16), 16, 8, L"Image\\platform.png"));

		//sprVineLeft.push(BitmapSpriteZero(RectangleShape(14, 32, 18, 0), 32, 32, 16, 16, L"Image\\walljump_l.png"));
		sprVineLeft.push(BitmapSpriteZero(32, 32, RectangleShape(14, 32, 18, 0), 16, 16, L"Image\\walljump_l.png"));

		sprVineRight.push(BitmapSpriteZero(32, 32, RectangleShape(14, 32, 0, 0), 16, 16, L"Image\\walljump_r.png"));

		sprBox.push(BitmapSpriteZero(32, 32, SquareShape(32), 16, 16, L"Image\\box.png"));

		//preset player sprites
		short i;
		wchar_t path[64];
		for (i = 0; i < BMPNUM_STANDING; i++)
		{
			swprintf_s(path, L"Image\\player_stand%d.png", i + 1);

			sprPlayerStanding.push(BitmapSpriteZero(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerStanding.setSpeed(BMPSPEED_STANDING);
		for (i = 0; i < BMPNUM_RUNNING; i++)
		{
			swprintf_s(path, L"Image\\player_running%d.png", i + 1);

			sprPlayerRunning.push(BitmapSpriteZero(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerRunning.setSpeed(BMPSPEED_RUNNING);
		for (i = 0; i < BMPNUM_JUMPING; i++)
		{
			swprintf_s(path, L"Image\\player_jump%d.png", i + 1);

			sprPlayerJumping.push(BitmapSpriteZero(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerJumping.setSpeed(BMPSPEED_JUMPING);
		for (i = 0; i < BMPNUM_FALLING; i++)
		{
			swprintf_s(path, L"Image\\player_fall%d.png", i + 1);

			sprPlayerFalling.push(BitmapSpriteZero(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerFalling.setSpeed(BMPSPEED_FALLING);
		for (i = 0; i < BMPNUM_SLIDING; i++)
		{
			swprintf_s(path, L"Image\\player_sliding%d.png", i + 1);

			sprPlayerSliding.push(BitmapSpriteZero(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerSliding.setSpeed(BMPSPEED_SLIDING);
	}
}