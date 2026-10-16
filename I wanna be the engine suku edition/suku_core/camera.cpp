#include "camera.h"
#include "object.h"
#include <algorithm>

namespace suku
{
	std::function<void(Camera*)> Camera::follow(Object* _object, float _velocity)
	{
		return [_object, _velocity](Camera* _c) {
			auto border = _c->getBorder();
			float targetX = std::clamp(_object->x.getValue().get(), border.left + _c->getCenterX(), border.right - _c->getCenterX());
			float targetY = std::clamp(_object->y.getValue().get(), border.top + _c->getCenterY(), border.bottom - _c->getCenterY());
			_c->x += (targetX - _c->x - _c->getCenterX()) * _velocity;
			_c->y += (targetY - _c->y - _c->getCenterY()) * _velocity;
			};
	}

	Transform Camera::getTransformToLayer()
	{
		return translation(-x, -y) + scale(x + centerX_, y + centerY_, xScale, yScale)
			+ rotation(x + centerX_, y + centerY_, angle) + extraTransform;
	}

	Transform Camera::getTransformToLayer(float _frameOffsetRate)
	{
		auto frameX = x.getInterpolatedFrameState(_frameOffsetRate);
		auto frameY = y.getInterpolatedFrameState(_frameOffsetRate);
		return translation(-frameX, -frameY)
			+ scale(frameX + centerX_, frameY + centerY_, xScale.getInterpolatedFrameState(_frameOffsetRate), yScale.getInterpolatedFrameState(_frameOffsetRate))
			+ rotation(frameX + centerX_, frameY + centerY_, angle.getInterpolatedFrameState(_frameOffsetRate))
			+ extraTransform;
	}

	void Camera::update()
	{
		if (isBinded_)
			bindFunc_(this);
	}

	void Camera::setBind(std::function<void(Camera*)> _func)
	{
		isBinded_ = true;
		bindFunc_ = _func;
	}

	void Camera::resetBind()
	{
		isBinded_ = false;
	}

	Camera::CameraBorder Camera::getBorder()
	{
		return { borderTop_, borderLeft_, borderBottom_, borderRight_ };
	}

	void Camera::setBorder(float _left, float _top, float _right, float _bottom)
	{
		borderTop_ = _top;
		borderLeft_ = _left;
		borderBottom_ = _bottom;
		borderRight_ = _right;
	}

	void Camera::resetBorder()
	{
		borderTop_ = 0.0f;
		borderBottom_ = FLT_MAX;
		borderLeft_ = 0.0f;
		borderRight_ = FLT_MAX;
	}
}