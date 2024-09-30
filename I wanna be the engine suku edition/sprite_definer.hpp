#pragma once
#include "basis.h"

namespace suku
{
	__declspec(selectany) Sprite
		sprWall, sprSpikeUp, sprSpikeDown, sprSpikeLeft, sprSpikeRight,
		sprWater, sprWaterExtraJump, sprBlood,
		sprTrigger, sprWarp, sprCherry, sprPlatform, sprVineLeft, sprVineRight,
		sprPlayerIdle, sprPlayerRunning, sprPlayerFalling, sprPlayerJumping, sprPlayerSliding,
		sprBox
		;

	inline void spritePreSet()
	{
		// preset other sprites
		sprCherry.push(BitmapSpriteZ(_T("Image\\cherry1.png"), 10, 12));

		sprCherry.push(BitmapSpriteZ(_T("Image\\cherry2.png"), 10, 12));

		sprCherry.setSpeed(25);

		//sprWall.push(BitmapSpriteZ(32, 32, SquareShape(32), 16, 16, _T("Image\\wall.png")));
		sprWall.push(BitmapSpriteZ(_T("Image\\wall.png"), 16, 16));

		sprSpikeUp.push(BitmapSpriteZ(_T("Image\\spike_u.png"), 16, 16));

		sprSpikeDown.push(BitmapSpriteZ(_T("Image\\spike_d.png"), 16, 16));

		sprSpikeLeft.push(BitmapSpriteZ(_T("Image\\spike_l.png"), 16, 16));

		sprSpikeRight.push(BitmapSpriteZ(_T("Image\\spike_r.png"), 16, 16));

		sprWater.push(BitmapSpriteZ(32, 32, SquareShape(32), 16, 16, _T("Image\\water_noextrajump.png")));

		sprWaterExtraJump.push(BitmapSpriteZ(32, 32, SquareShape(32), 16, 16, _T("Image\\water_extrajump.png")));

		sprTrigger.push(ShapeSpriteZ(SquareShape(32)));

		sprBlood.push(BitmapSpriteZ(2, 2, SquareShape(2), 1, 1, _T("Image\\blood.png")));

		sprWarp.push(BitmapSpriteZ(_T("Image\\warp.png"), 16, 16));

		sprPlatform.push(BitmapSpriteZ(32, 16, RectangleShape(32, 16), 16, 8, _T("Image\\platform.png")));

		//sprVineLeft.push(BitmapSpriteZ(RectangleShape(14, 32, 18, 0), 32, 32, 16, 16, _T("Image\\walljump_l.png")));
		sprVineLeft.push(BitmapSpriteZ(32, 32, RectangleShape(14, 32, 18, 0), 16, 16, _T("Image\\walljump_l.png")));

		sprVineRight.push(BitmapSpriteZ(32, 32, RectangleShape(14, 32, 0, 0), 16, 16, _T("Image\\walljump_r.png")));

		sprBox.push(BitmapSpriteZ(32, 32, SquareShape(32), 16, 16, _T("Image\\box.png")));

		//preset player sprites
		short i;
		TCHAR path[64];
		for (i = 0; i < BMPNUM_STANDING; i++)
		{
			_stprintf_s(path, _T("Image\\player_stand%d.png"), i + 1);

			sprPlayerIdle.push(BitmapSpriteZ(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerIdle.setSpeed(BMPSPEED_STANDING);
		for (i = 0; i < BMPNUM_RUNNING; i++)
		{
			_stprintf_s(path, _T("Image\\player_running%d.png"), i + 1);

			sprPlayerRunning.push(BitmapSpriteZ(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerRunning.setSpeed(BMPSPEED_RUNNING);
		for (i = 0; i < BMPNUM_JUMPING; i++)
		{
			_stprintf_s(path, _T("Image\\player_jump%d.png"), i + 1);

			sprPlayerJumping.push(BitmapSpriteZ(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerJumping.setSpeed(BMPSPEED_JUMPING);
		for (i = 0; i < BMPNUM_FALLING; i++)
		{
			_stprintf_s(path, _T("Image\\player_fall%d.png"), i + 1);

			sprPlayerFalling.push(BitmapSpriteZ(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerFalling.setSpeed(BMPSPEED_FALLING);
		for (i = 0; i < BMPNUM_SLIDING; i++)
		{
			_stprintf_s(path, _T("Image\\player_sliding%d.png"), i + 1);

			sprPlayerSliding.push(BitmapSpriteZ(PLAYER_WIDTH, PLAYER_HEIGHT,
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerSliding.setSpeed(BMPSPEED_SLIDING);
	}
}