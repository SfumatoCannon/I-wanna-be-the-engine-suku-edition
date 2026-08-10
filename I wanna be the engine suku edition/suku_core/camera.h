#pragma once


namespace suku
{
	class Room;
	class Object;
	class PaintLayer;

	class Camera
	{
	public:
		float getX() { return x_; }
		float getY() { return y_; }
		float getCenterX();
		float getCenterY();
	private:
		PaintLayer* bindedLayer_;
		float x_ = 0.0f;
		float y_ = 0.0f;
		float angle_ = 0.0f;
	};
}