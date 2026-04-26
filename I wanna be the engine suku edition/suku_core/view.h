#pragma once
#include "room.h"
#include "../suku_constants.h"


namespace suku
{
	class View
	{
	public:
	private:
		Room& targetRoom_;
		float x_ = 0.0f;
		float y_ = 0.0f;
		float width_ = constants::window::widthLogical;
		float height_ = constants::window::heightLogical;
	};
}