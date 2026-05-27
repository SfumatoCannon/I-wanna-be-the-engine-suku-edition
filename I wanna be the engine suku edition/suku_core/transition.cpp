#include "pch.h"
#include "transition.h"

namespace suku
{
	const TransitionCurve TransitionCurve::linear([](double t) { return t; });
	const TransitionCurve TransitionCurve::easeInQuad([](double t) { return t * t; });
	const TransitionCurve TransitionCurve::easeOutQuad([](double t) { return t * (2 - t); });
	const TransitionCurve TransitionCurve::easeInOutQuad([](double t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t; });
	const TransitionCurve TransitionCurve::easeInCubic([](double t) { return t * t * t; });
	const TransitionCurve TransitionCurve::easeOutCubic([](double t) { return (--t) * t * t + 1; });
	const TransitionCurve TransitionCurve::easeInOutCubic([](double t) { return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; });
	const TransitionCurve TransitionCurve::easeInExpo([](double t) { return t == 0 ? 0 : pow(2, 10 * (t - 1)); });
	const TransitionCurve TransitionCurve::easeOutExpo([](double t) { return t == 1 ? 1 : 1 - pow(2, -10 * t); });
	const TransitionCurve TransitionCurve::easeInOutExpo([](double t) { return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2; });
	
	Transition::Transition(const Transition& _other)
		:duration_(_other.duration_), curve_(_other.curve_)
	{
	}

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