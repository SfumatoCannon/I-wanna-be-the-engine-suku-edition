#pragma once
#include "suku_core/object.h"
#include <utility>

namespace suku
{
	class UIElement : public Object
	{
	public:
		UIElement(float _x = 0, float _y = 0, int _width = 32, int _height = 32);
		virtual void update() override;
		virtual bool onPaint() override;
		int getWidth() const { return width_; }
		void setWidth(int _width) { width_ = _width; }
		int getHeight() const { return height_; }
		void setHeight(int _height) { height_ = _height; }
		std::pair<int, int> getSize() const { return { width_, height_ }; }
	protected:
		int width_, height_;
	};
}