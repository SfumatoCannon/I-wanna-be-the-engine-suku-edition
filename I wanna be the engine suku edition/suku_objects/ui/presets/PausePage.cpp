#include "PausePage.h"
#include "../../../suku_gamewindow/includes.h"
#include "../../../suku_foundation/input.h"

namespace suku
{
	PausePage::PausePage()
		: UIElement(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight())
	{

	}

	void PausePage::update()
	{
		if (input::isKeyDown(input::VK_P))
		{
			isPaused_ = !isPaused_;
		}
	}

	bool PausePage::onPaint()
	{
		if (isPaused_)
		{
			Text pauseText("Consolas", 64, TextAlign::MiddleCenter);
			RectangleShape background(GameWindow::getLogicalWidth() + 1, GameWindow::getLogicalHeight() + 1);
			background.setFill(Color(0, 0, 0, 0.5f));
			background.paint(0, 0);
			pauseText.contentString = L"PAUSED";
			pauseText.setBrush(Color(255, 255, 255));
			pauseText.paint(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight());
		}
		return true;
	}
}