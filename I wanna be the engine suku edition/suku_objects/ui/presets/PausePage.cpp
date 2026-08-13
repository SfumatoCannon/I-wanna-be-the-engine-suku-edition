#include "PausePage.h"
#include "../../../suku_gamewindow/includes.h"
#include "../../../suku_foundation/input.h"

namespace suku
{
	PausePage::PausePage()
		: UIElement(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight())
	{
		isVisible = false;
		setPaintId(1000);
	}

	void PausePage::onUpdate()
	{
		if (input::isKeyDown(input::VK_P))
		{
			if (!isPaused_)
			{
				isPaused_ = true;
				isVisible = true;
				if (inRoom_)
					inRoom_->pause(this);
			}
			else
			{
				isPaused_ = false;
				isVisible = false;
				if (inRoom_)
					inRoom_->resume();
			}
		}
	}

	bool PausePage::onPaint()
	{
		Text pauseText("Consolas", 64, TextAlign::MiddleCenter);
		RectangleShape background(GameWindow::getLogicalWidth() + 1, GameWindow::getLogicalHeight() + 1);
		background.setFill(Color(0, 0, 0, 0.5f));
		background.paint(0, 0);
		pauseText.contentString = L"PAUSED";
		pauseText.setBrush(Color(255, 255, 255));
		pauseText.paint(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight());
		return true;
	}
}