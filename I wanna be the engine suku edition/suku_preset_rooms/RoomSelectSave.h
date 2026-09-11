#pragma once

#include <I wanna be the engine suku edition.h>
#include "skins.hpp"

using namespace suku;

class RoomSelectSave : public RoomStage0
{
public:
	inline static SaveFile saveFile[3] = {SaveFile("Save1"), SaveFile("Save2"), SaveFile("Save3")};
	RoomSelectSave();
	virtual void onUpdateStart() override;
	virtual void onPaintEnd(PaintLayer& _layer) override;
private:
	int selectedSaveIndex_ = 0;
};