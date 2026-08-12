#pragma once

#include "../suku_constants.h"
#include "suku_property.h"
#include <functional>

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

		void setCenterX(float _centerX) { centerX_ = _centerX; }
		void setCenterY(float _centerY) { centerY_ = _centerY; }

		void update();
		bool isBinded() { return isBinded_; }
		void setBind(std::function<void(Camera*)> _func);
		void resetBind();

		static std::function<void(Camera*)> follow(Object* _object, float _velocity /* 0 ~ 1 */);

		Property<float> x = 0.0f;
		Property<float> y = 0.0f;
		Property<float> angle = 0.0f;
	private:
		std::function<void(Camera*)> bindFunc_;
		bool isBinded_ = false;
		float width_ = constants::window::widthLogical;
		float height_ = constants::window::heightLogical;
		float centerX_ = constants::window::widthLogical / 2;
		float centerY_ = constants::window::heightLogical / 2;
	};
}