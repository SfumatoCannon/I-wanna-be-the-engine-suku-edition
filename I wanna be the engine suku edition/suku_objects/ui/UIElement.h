#pragma once
#include "suku_core/object.h"
#include <utility>

namespace suku
{
	class UIElement : public Object
	{
	public:
		UIElement(real _x = 0, real _y = 0, real _width = 32, real _height = 32);
		virtual void onUpdate() override;
		virtual void onPaint() override;

		real getWidth() const { return width_; }
		real getHeight() const { return height_; }
		std::pair<real, real> getSize() const { return { width_, height_ }; }

		UIElement* setWidth(real _width) { width_ = _width; return this; }
		UIElement* setHeight(real _height) { height_ = _height; return this; }
		UIElement* setSize(real _width, real _height) { width_ = _width, height_ = _height; return this; }

		void setTextStyle(const TextStyle& _font);
		// alias
			void setFont(const TextStyle& _font) { setTextStyle(_font); }

	protected:
		inline static TextStyle defaultTextStyle_{ "Arial", 16, TextStyle::Weight::Bold };
		real width_, height_;
		const TextStyle& textStyle_ = defaultTextStyle_;
	};
}