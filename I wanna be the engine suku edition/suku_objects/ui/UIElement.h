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

		real getWidth()const { return width_; }
		real getHeight()const { return height_; }
		std::pair<real, real> getSize()const { return { width_, height_ }; }

		UIElement* setWidth(real _width) { width_ = _width; updateArea(); return this; }
		UIElement* setHeight(real _height) { height_ = _height; updateArea(); return this; }
		UIElement* setSize(real _width, real _height) { width_ = _width, height_ = _height; updateArea(); return this; }

		void setTextStyle(const TextStyle& _font);
		// alias
			void setFont(const TextStyle& _font) { setTextStyle(_font); }
		
		const Shape& getArea() const { return area_; }

		void setBackgroundBrush(const Brush& _brush) { backgroundBrush_ = _brush; }
		const Brush& getBackgroundBrush()const { return backgroundBrush_; }
		void setOutlineBrush(const Brush& _brush) { outlineBrush_ = _brush; }
		const Brush& getOutlineBrush()const { return outlineBrush_; }

		void paintArea()const;
		void paintBackground()const;
		void paintOutline()const;
	protected:
		inline static TextStyle defaultTextStyle_{ "Arial", 16, TextStyle::Weight::Bold };
		real width_, height_;
		Shape area_;
		Brush backgroundBrush_ = Brush::solidColorBrush(Color(0, 0, 0, 0.3));
		Brush outlineBrush_ = Brush::emptyBrush();
		const TextStyle& textStyle_ = defaultTextStyle_;
	private:
		void updateArea();
	};
}