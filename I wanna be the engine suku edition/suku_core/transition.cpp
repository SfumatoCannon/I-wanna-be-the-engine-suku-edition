#include "pch.h"
#include "transition.h"

namespace suku
{
	Transition::Transition(double _duration, const TransitionCurve& _function)
		: duration_(_duration), curve_(_function)
	{
	}

	Transition::Transition(double _duration, TransitionCurve&& _function)
		: duration_(_duration), curve_(std::forward<TransitionCurve>(_function))
	{
	}

	Transition::Transition(double _duration, const std::function<double(double)>& _function)
		: duration_(_duration), curve_(TransitionCurve(_function))
	{
	}

	double Transition::getValue(double beginValue, double endValue, double elapsedTime) const
	{
		double x = std::clamp(elapsedTime / duration_, 0.0, 1.0);
		return beginValue + (endValue - beginValue) * curve_.getValue(x);
	}

	TransitionCurve::TransitionCurve(const std::function<double(double)>& _function)
		: function_(_function)
	{
	}

	TransitionCurve::TransitionCurve(std::function<double(double)>&& _function)
		: function_(std::move(_function))
	{
	}
}