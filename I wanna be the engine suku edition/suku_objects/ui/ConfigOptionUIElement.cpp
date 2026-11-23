#include "ConfigOptionUIElement.h"

namespace suku
{
	void ConfigOptionUIElement::focus()
	{
		isFocused_ = true;
		if (hasInfo_)
			height_ = originalHeight_ + 8 + infoText_.getContentHeight(getWidth() - 16);
	}

	void ConfigOptionUIElement::unfocus()
	{
		isFocused_ = false;
		height_ = originalHeight_;
	}

	void ConfigOptionUIElement::onUpdate()
	{
		if (!isFocused_)
			return;

		if (input::isKeyDown(VK_LEFT))
		{
			pBindedConfig_->selectPrev();
		}
		else if (input::isKeyDown(VK_RIGHT))
		{
			pBindedConfig_->selectNext();
		}
	}

	void ConfigOptionUIElement::onPaint()
	{
		RectangleShape area(getWidth(), getHeight());
		area.setFill(isFocused_ ? Color(128, 128, 128, 0.5f) : Color(64, 64, 64, 0.5f));
		area.paint(x, y, getTransform());

		// label text
		labelText_.style.setBrush(Color(255, 255, 255));
		labelText_.text = label_;
		labelText_.paint(x + 16, y + 8, getWidth() - 32, originalHeight_ - 16);

		// value text
		valueText_.style.setBrush(Color(255, 255, 255));
		valueText_.text = pBindedConfig_->getValueString();
		valueText_.paint(x + 16, y + 8, getWidth() - 32, originalHeight_ - 16);

		if (hasInfo_ && isFocused_)
		{
			float height = infoText_.getContentHeight(getWidth() - 16);
			infoText_.paint(x + 16, y + originalHeight_ + 4, getWidth() - 32, height);
		}
	}
}