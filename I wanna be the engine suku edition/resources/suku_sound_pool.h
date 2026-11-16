#pragma once
#include <suku_sounds/suku_sounds.h>

namespace suku::resource
{
	class Audio
	{
	public:
		static inline Sound musGuyRock{ "Audio\\musGuyRock.mp3", 0.3f };


		static inline Sound sndShoot{ "Audio\\sndShoot.wav" , 0.2f};
		static inline Sound sndDeath{ "Audio\\sndDeath.wav" , 0.2f};
		static inline Sound sndJump{ "Audio\\sndJump.wav" };
		static inline Sound sndDoubleJump{ "Audio\\sndDJump.wav" };
		static inline Sound sndVineJump{ "Audio\\sndWallJump.wav" };
	};
}