#pragma once
#include <functional>
#include <cmath>
#include <memory>

namespace suku
{
	class TransitionCurve
	{
	public:
		TransitionCurve() = default;
		TransitionCurve(const std::function<double(double)>& _function);
		TransitionCurve(std::function<double(double)>&& _function);
		
		double operator()(double t) const { return function_(t); }
		double getValue(double t) const { return function_(t); }
	private:
		std::function<double(double)> function_;
	};

	const TransitionCurve linear([](double t) { return t; });
	const TransitionCurve easeInQuad([](double t) { return t * t; });
	const TransitionCurve easeOutQuad([](double t) { return t * (2 - t); });
	const TransitionCurve easeInOutQuad([](double t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t; });
	const TransitionCurve easeInCubic([](double t) { return t * t * t; });
	const TransitionCurve easeOutCubic([](double t) { return (--t) * t * t + 1; });
	const TransitionCurve easeInOutCubic([](double t) { return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; });
	const TransitionCurve easeInExpo([](double t) { return t == 0 ? 0 : pow(2, 10 * (t - 1)); });
	const TransitionCurve easeOutExpo([](double t) { return t == 1 ? 1 : 1 - pow(2, -10 * t); });
	const TransitionCurve easeInOutExpo([](double t) { return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2; });

	class Transition
	{
	public:
		Transition(double _duration, const TransitionCurve& _curve = linear);
		Transition(double _duration, TransitionCurve&& _curve);
		Transition(double _duration, const std::function<double(double)>& _curveFunction);
		double getValue(double beginValue, double endValue, double elapsedTime) const;
	private:
		double duration_;
		const TransitionCurve& curve_;
	};
}