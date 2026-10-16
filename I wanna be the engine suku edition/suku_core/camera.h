#pragma once

#include "../suku_constants.h"
#include "suku_property.h"
#include <functional>
#include <suku_draw/transform.h>

namespace suku
{
	class Room;
	class Object;
	class PaintLayer;

	class Camera
	{
	public:
		Camera() = default;
		Camera(float _width, float _height) : width_(_width), height_(_height) {}

		float getX() { return x; }
		float getY() { return y; }
		float getCenterX() { return centerX_; }
		float getCenterY() { return centerY_; }
		float getWidth() { return width_; }
		float getHeight() { return height_; }
		Transform getTransformToLayer();
		Transform getTransformToLayer(float _frameOffsetRate);

		void setCenterX(float _centerX) { centerX_ = _centerX; }
		void setCenterY(float _centerY) { centerY_ = _centerY; }

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
		void setBorder(float _left, float _top, float _right, float _bottom);
		void resetBorder();


		static std::function<void(Camera*)> follow(Object* _object, float _velocity /* 0 ~ 1 */);

		Property<real> x = 0.0f;
		Property<real> y = 0.0f;
		Property<real> xScale = 1, yScale = 1;
		Property<real> angle = 0.0f;
		Transform extraTransform = Transform();
	private:
		std::function<void(Camera*)> bindFunc_;
		bool isBinded_ = false;
		float width_ = constants::window::widthLogical;
		float height_ = constants::window::heightLogical;
		float centerX_ = constants::window::widthLogical / 2;
		float centerY_ = constants::window::heightLogical / 2;
		float borderTop_ = 0.0f;
		float borderBottom_ = FLT_MAX;
		float borderLeft_ = 0.0f;
		float borderRight_ = FLT_MAX;
	};
}