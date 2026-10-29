#pragma once

#include "../suku_constants.h"
#include "suku_property.h"
#include <functional>
#include <suku_draw/transform.h>
#include <suku_foundation/maths.h>

namespace suku
{
	class Room;
	class Object;
	class PaintLayer;

	class Camera
	{
	public:
		Camera() = default;
		Camera(real _width, real _height) : width_(_width), height_(_height) {}

		real getX() { return x; }
		real getY() { return y; }
		real getCenterX() { return centerX_; }
		real getCenterY() { return centerY_; }
		real getWidth() { return width_; }
		real getHeight() { return height_; }
		Transform getTransformToLayer();
		Transform getTransformToLayer(real _frameOffsetRate);

		void setCenterX(real _centerX) { centerX_ = _centerX; }
		void setCenterY(real _centerY) { centerY_ = _centerY; }

		void update();
		bool isBinded() { return isBinded_; }
		void setBind(std::function<void(Camera*)> _func);
		void resetBind();

		struct CameraBorder
		{
			real top;
			real left;
			real bottom;
			real right;
		};

		CameraBorder getBorder();
		void setBorder(real _left, real _top, real _right, real _bottom);
		void resetBorder();


		static std::function<void(Camera*)> follow(Object* _object, real _velocity /* 0 ~ 1 */);

		Property<real> x = 0.0f;
		Property<real> y = 0.0f;
		Property<real> xScale = 1, yScale = 1;
		Property<real> angle = 0.0f;
		Transform extraTransform = Transform();
	private:
		std::function<void(Camera*)> bindFunc_;
		bool isBinded_ = false;
		real width_ = constants::window::widthLogical;
		real height_ = constants::window::heightLogical;
		real centerX_ = constants::window::widthLogical / 2;
		real centerY_ = constants::window::heightLogical / 2;
		real borderTop_ = 0.0f;
		real borderBottom_ = FLT_MAX;
		real borderLeft_ = 0.0f;
		real borderRight_ = FLT_MAX;
	};
}