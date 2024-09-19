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
		sprCherry.push({ 21, 24, 0, 0, 21, 24, 10, 12, true, _T("Image\\cherry1.png") });

		sprCherry.push({ 21, 24, 0, 0, 21, 24, 10, 12, true, _T("Image\\cherry2.png") });

		sprCherry.setSpeed(25);

		sprWall.push(BitmapSpriteZ(SquareShape(32), 32, 32, 16, 16, _T("Image\\wall.png")));

		sprSpikeUp.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, true, _T("Image\\spike_u.png")));

		sprSpikeDown.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, true, _T("Image\\spike_d.png")));

		sprSpikeLeft.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, true, _T("Image\\spike_l.png")));

		sprSpikeRight.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, true, _T("Image\\spike_r.png")));

		sprWater.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, false, _T("Image\\water_noextrajump.png")));

		sprWaterExtraJump.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, false, _T("Image\\water_extrajump.png")));

		sprTrigger.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, false, nullptr));

		sprBlood.push(BitmapSpriteZ(BLOOD_WIDTH, BLOOD_HEIGHT, BLOOD_PDX, BLOOD_PDY, BLOOD_PDWIDTH, BLOOD_PDHEIGHT,
			0, 0, false, _T("Image\\blood.png")));

		sprWarp.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, true, _T("Image\\warp.png")));

		sprPlatform.push(BitmapSpriteZ(32, 16, 0, 0, 32, 16, 16, 8, false, _T("Image\\platform.png")));

		//sprVineLeft.push(BitmapSpriteZ(RectangleShape(14, 32, 18, 0), 32, 32, 16, 16, _T("Image\\walljump_l.png")));
		sprVineLeft.push(BitmapSpriteZ(32, 32, 18, 0, 14, 32, 0, 0, false, _T("Image\\walljump_l.png")));

		sprVineRight.push(BitmapSpriteZ(32, 32, 0, 0, 14, 32, 0, 0, false, _T("Image\\walljump_r.png")));

		sprBox.push(BitmapSpriteZ(32, 32, 0, 0, 32, 32, 16, 16, false, _T("Image\\box.png")));

		//preset player sprites
		short i;
		TCHAR path[64];
		for (i = 0; i < BMPNUM_STAND; i++)
		{
			_stprintf_s(path, _T("Image\\player_stand%d.png"), i + 1);

			sprPlayerIdle.push(BitmapSpriteZ(
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_WIDTH, PLAYER_HEIGHT,
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerIdle.setSpeed(BMPSPEED_STAND);
		for (i = 0; i < BMPNUM_RUNNING; i++)
		{
			_stprintf_s(path, _T("Image\\player_running%d.png"), i + 1);

			sprPlayerRunning.push(BitmapSpriteZ(
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_WIDTH, PLAYER_HEIGHT,
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerRunning.setSpeed(BMPSPEED_RUNNING);
		for (i = 0; i < BMPNUM_JUMP; i++)
		{
			_stprintf_s(path, _T("Image\\player_jump%d.png"), i + 1);

			sprPlayerJumping.push(BitmapSpriteZ(
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_WIDTH, PLAYER_HEIGHT,
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerJumping.setSpeed(BMPSPEED_JUMP);
		for (i = 0; i < BMPNUM_FALL; i++)
		{
			_stprintf_s(path, _T("Image\\player_fall%d.png"), i + 1);

			sprPlayerFalling.push(BitmapSpriteZ(
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_WIDTH, PLAYER_HEIGHT,
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerFalling.setSpeed(BMPSPEED_FALL);
		for (i = 0; i < BMPNUM_SLIDING; i++)
		{
			_stprintf_s(path, _T("Image\\player_sliding%d.png"), i + 1);

			sprPlayerSliding.push(BitmapSpriteZ(
				RectangleShape(PLAYER_PDWIDTH, PLAYER_PDHEIGHT, PLAYER_PDX, PLAYER_PDY),
				PLAYER_WIDTH, PLAYER_HEIGHT,
				PLAYER_CENTERX, PLAYER_CENTERY, path));
		}
		sprPlayerSliding.setSpeed(BMPSPEED_SLIDING);
	}
}