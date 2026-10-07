#include "Save.h"
#include "player.h"

namespace suku
{
	void Save::onUpdateEnd()
	{
		if (inRoom_->getCrashedObject<Player>(this) && input::isKeyDown(VK_Z) 
			&& (lastSaved_ == -1 || clock_ - lastSaved_ >= 30))
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