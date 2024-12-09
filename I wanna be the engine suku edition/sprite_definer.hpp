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

		sprBlood.push(BitmapSpriteZero(L"Image\\blood.png", SquareShape(2)));

		sprWarp.push(BitmapSpriteZero(L"Image\\warp.png", 16, 16));

		sprPlatform.push(BitmapSpriteZero(L"Image\\platform.png", RectangleShape(32, 16)));

		//sprVineLeft.push(BitmapSpriteZero(RectangleShape(14, 32, 18, 0), 32, 32, 16, 16, L"Image\\walljump_l.png"));
		sprVineLeft.push(BitmapSpriteZero(L"Image\\walljump_l.png", RectangleShape(14, 32, 18, 0)));

		sprVineRight.push(BitmapSpriteZero(L"Image\\walljump_r.png", RectangleShape(14, 32, 0, 0)));

		sprBox.push(BitmapSpriteZero(L"Image\\box.png", SquareShape(32)));

		//preset player sprites
		short i;
		wchar_t path[64];
		for (i = 0; i < BMPNUM_STANDING; i++)
		{
			swprintf_s(path, L"Image\\player_stand%d.png", i + 1);

			sprPlayerStanding.push(BitmapSpriteZero(path,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY), PLAYER_CENTERX, PLAYER_CENTERY));
		}
		sprPlayerStanding.setSpeed(BMPSPEED_STANDING);
		for (i = 0; i < BMPNUM_RUNNING; i++)
		{
			swprintf_s(path, L"Image\\player_running%d.png", i + 1);

			sprPlayerRunning.push(BitmapSpriteZero(path,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY), PLAYER_CENTERX, PLAYER_CENTERY));
		}
		sprPlayerRunning.setSpeed(BMPSPEED_RUNNING);
		for (i = 0; i < BMPNUM_JUMPING; i++)
		{
			swprintf_s(path, L"Image\\player_jump%d.png", i + 1);

			sprPlayerJumping.push(BitmapSpriteZero(path,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY), PLAYER_CENTERX, PLAYER_CENTERY));
		}
		sprPlayerJumping.setSpeed(BMPSPEED_JUMPING);
		for (i = 0; i < BMPNUM_FALLING; i++)
		{
			swprintf_s(path, L"Image\\player_fall%d.png", i + 1);

			sprPlayerFalling.push(BitmapSpriteZero(path,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY), PLAYER_CENTERX, PLAYER_CENTERY));
		}
		sprPlayerFalling.setSpeed(BMPSPEED_FALLING);
		for (i = 0; i < BMPNUM_SLIDING; i++)
		{
			swprintf_s(path, L"Image\\player_sliding%d.png", i + 1);

			sprPlayerSliding.push(BitmapSpriteZero(path,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY), PLAYER_CENTERX, PLAYER_CENTERY));
		}
		sprPlayerSliding.setSpeed(BMPSPEED_SLIDING);
	}
}