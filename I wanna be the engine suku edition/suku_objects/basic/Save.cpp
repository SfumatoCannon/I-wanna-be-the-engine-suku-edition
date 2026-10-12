#include "Save.h"
#include "player.h"

namespace suku
{
	void Save::onUpdateEnd()
	{
		bool saveAction = false;
		if ((getCrashedObject<Player>() && input::isKeyDown(VK_Z)) || getCrashedObject<Bullet>())
		{
			saveAction = true;
		}

		if (saveAction && (lastSaved_ == -1 || clock_ - lastSaved_ >= 30))
		{
			spriteBasicIndex = 1;
			SaveFile::save();
			lastSaved_ = this->clock_;
		}
		if (clock_ - lastSaved_ > 60)
		{
			spriteBasicIndex = 0;
		}
	}
}