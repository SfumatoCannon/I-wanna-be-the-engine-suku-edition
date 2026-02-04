#pragma once
#include "room.h"


namespace suku
{
	class View
	{
	public:
	private:
		Room& targetRoom_;
		float x_ = 0.0f;
		float y_ = 0.0f;
		float width_ = 800.0f;
		float height_ = 608.0f;
	};
}