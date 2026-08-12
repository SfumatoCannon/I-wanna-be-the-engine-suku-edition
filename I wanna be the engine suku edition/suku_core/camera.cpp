#include "camera.h"
#include "object.h"

namespace suku
{
	std::function<void(Camera*)> Camera::follow(Object* _object, float _velocity)
	{
		return [_object, _velocity](Camera* _c) {
			_c->x += (_object->x - _c->x - _c->getCenterX()) * _velocity;
			_c->y += (_object->y - _c->y - _c->getCenterY()) * _velocity;
			};
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
}