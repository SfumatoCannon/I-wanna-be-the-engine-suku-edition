#include "ConfigOptionUIElement.h"

namespace suku
{
	void ConfigOptionUIElement::focus()
	{
		isFocused_ = true;
		if (hasInfo_)
			height_ = originalHeight_ + 8 + infoText_.getContentHeight(getWidth() - 16);
	}

	bool ConfigOptionUIElement::unfocus()
	{
		isFocused_ = false;
		height_ = originalHeight_;
		return true;
	}
}