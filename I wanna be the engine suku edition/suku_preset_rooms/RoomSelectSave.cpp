#include "RoomSelectSave.h"
#include <resources/suku_font_pool.h>

namespace
{
	constexpr int saveSlotY = 160;
	constexpr int saveSlotX[3] = { 96, 336, 576 };
	constexpr int saveSlotWidth = 128;
	constexpr int saveSlotHeight = 288;
	constexpr int padding = 8;
	ChoiceGroupUIElement* choiceUIElement[3];
}



RoomSelectSave::RoomSelectSave()
{
	MapLoader::loadFromJtoolData(this, "-801p01o01n01m01hg1gg1fg1eg1a01901801701ig1dg1l01q01b01601s0140-i01eg1fg1gg1hg1ig1dg1q01701601o01n01m01l01p01b01a01901801s0140-h01ig1dg1l01q01b01601s0140-g01ig1dg1l01q01b01601s0140-f01ig1dg1l01q01b01601s0140-e01ig1dg1l01q01b01601s0140-d01ig1dg1l01q01b01601s0140-c01ig1dg1l01q01b01601s0140-b01ig1dg1l01q01b01601s0140-a01ig1dg1l01q01b01601s0140-901ig1dg1l01q01b01601s0140-l01s0140-k01s0140-j01s0140-701s0140-601s0140-501s0140-401s01r01q01p01o01n01m01l01k01j01i01h01g01f01e01d01c01b01a0190180170160150140-m01s01r01q01p01o01n01m01l01k01j01i01h01g01f01e01d01c01b01a0190180170160150140");
	for (int i = 0; i < 3; i++)
	{
		saveFileTime[i] = saveFile[i].get<unsigned int>("time");
		saveFileDeath[i] = saveFile[i].get<unsigned int>("death");
		choiceUIElement[i] = create<ChoiceGroupUIElement>(saveSlotX[i], saveSlotY + 48, saveSlotWidth, 32,
			std::vector<String>{"Load Game", "Medium", "Hard", "Very Hard"}, 0).get();
		choiceUIElement[i]->isVisible = false;
		choiceUIElement[i]->unfocus();
	}
}

void RoomSelectSave::onUpdateStart()
{
	auto choiceUI = choiceUIElement[selectedSaveIndex_];
	if (input::isKeyDown(VK_LEFT_ARROW))
	{
		if (choiceUI->isVisible == false)
		{
			if (selectedSaveIndex_ > 0)
				selectedSaveIndex_--;
		}
	}
	else if (input::isKeyDown(VK_RIGHT_ARROW))
	{
		if (choiceUI->isVisible == false)
		{
			if (selectedSaveIndex_ < 2)
				selectedSaveIndex_++;
		}
	}
	else if (input::isKeyDown(VK_SHIFT))
	{
		if (choiceUI->isVisible == true)
		{
			auto choice = choiceUI->getCurrentIndex();
			if (choice == 0) // Load Game
			{
				setGlobalSaveFile(&saveFile[selectedSaveIndex_]);
				SaveFile::load();
			}
			else if (choice == 1 || choice == 2 || choice == 3) // Medium | Hard | Very Hard
			{
				setGlobalSaveFile(&saveFile[selectedSaveIndex_]);
				RoomPool::gotoNewGameRoom();
			}
		}
		else // choiceUI->isVisible == false
		{
			choiceUI->isVisible = true;
			choiceUI->focus();
		}
	}

	if (input::isKeyDown(VK_Z))
	{
		if (choiceUI->isVisible == true)
		{
			choiceUI->unfocus();
			choiceUI->isVisible = false;
		}
	}
}

void RoomSelectSave::onPaintEnd(PaintLayer& _layer)
{
	TextStyle saveSlotTitleStyle(resource::Font::Staatliches, 32, TextStyle::Align::TopCenter);
	TextStyle saveSlotNoDataStyle(resource::Font::Barlow_Condensed, 24, TextStyle::Align::TopCenter);
	TextStyle saveSlotContentStyle(resource::Font::Arial, 16, TextStyle::Weight::Bold);


	// Save slot card
	for (int i = 0; i < 3; i++)
	{
		// Title
		saveSlotTitleStyle.paint(L"Save" + std::to_wstring(i + 1),
			saveSlotX[i], saveSlotY + padding, saveSlotWidth, saveSlotHeight);

		// Info
		if (!saveFile[i].isExist())
		{
			saveSlotNoDataStyle.paint(L"No Data",
				saveSlotX[i], saveSlotY + padding + 96, saveSlotWidth, saveSlotHeight - 96,
				Brush::solidColorBrush(Color(100, 100, 100)));
		}
		else
		{
			auto time = saveFileTime[i];
			auto death = saveFileDeath[i];
			time = time / game_loop::updateFPS; // frame -> seconds
			unsigned int hour = time / 3600;
			unsigned int minute = (time % 3600) / 60;
			unsigned int second = time % 60;
			saveSlotContentStyle.setTextAlign(TextStyle::Align::TopLeft);
			saveSlotContentStyle.paint(
				L"Time\nDeath",
				saveSlotX[i] + padding, saveSlotY + padding + 96, saveSlotWidth, saveSlotHeight - 96);
			saveSlotContentStyle.setTextAlign(TextStyle::Align::TopRight);
			saveSlotContentStyle.paint(
				std::to_wstring(hour) + L":" + std::to_wstring(minute) + L":" + std::to_wstring(second) + L"\n" + std::to_wstring(death),
				saveSlotX[i] + padding, saveSlotY + padding + 96, saveSlotWidth - padding * 2, saveSlotHeight - 96);
		}
	}

	// show selectedSaveIndex_
	int startX = saveSlotX[selectedSaveIndex_];
	int startY = saveSlotY + saveSlotHeight + 64;
	int mid = startX + saveSlotWidth / 2;
	for (int i = startX - 16; i < mid; i += 16)
		Cherry::spr.getFrameState(clock_)->paintWithCenter(i, startY);
	for (int i = mid + 16; i < startX + saveSlotWidth + 32; i += 16)
		Cherry::spr.getFrameState(clock_)->paintWithCenter(i, startY);
	Player::sprStanding.getFrameState(clock_)->paintWithCenter(mid, startY);

}
