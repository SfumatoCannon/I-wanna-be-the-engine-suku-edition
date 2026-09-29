#pragma once
#include "suku_core/object.h"
#include <utility>

namespace suku
{
	class UIElement : public Object
	{
	public:
		UIElement(float _x = 0, float _y = 0, int _width = 32, int _height = 32);
		virtual void onUpdate() override;
		virtual void onPaint() override;

		int getWidth() const { return width_; }
		int getHeight() const { return height_; }
		std::pair<int, int> getSize() const { return { width_, height_ }; }

		UIElement* setWidth(int _width) { width_ = _width; return this; }
		UIElement* setHeight(int _height) { height_ = _height; return this; }
		UIElement* setSize(int _width, int _height) { width_ = _width, height_ = _height; return this; }

		void setTextStyle(const TextStyle& _font);
		// alias
			void setFont(const TextStyle& _font) { setTextStyle(_font); }

	protected:
		inline static TextStyle defaultTextStyle_{ "Arial", 16, TextStyle::Weight::Bold };
		int width_, height_;
		const TextStyle& textStyle_ = defaultTextStyle_;
	};
}