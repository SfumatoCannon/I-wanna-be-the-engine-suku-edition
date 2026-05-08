#pragma once
#include <functional>
#include <cmath>
#include <memory>

namespace suku
{
	class TransitionFunction
	{
	public:
		TransitionFunction() = default;
		TransitionFunction(const std::function<double(double)>& _function) : function_(_function) {}
	private:
		std::function<double(double)> function_;
	};

	const TransitionFunction linear([](double t) { return t; });
	const TransitionFunction easeInQuad([](double t) { return t * t; });
	const TransitionFunction easeOutQuad([](double t) { return t * (2 - t); });
	const TransitionFunction easeInOutQuad([](double t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t; });
	const TransitionFunction easeInCubic([](double t) { return t * t * t; });
	const TransitionFunction easeOutCubic([](double t) { return (--t) * t * t + 1; });
	const TransitionFunction easeInOutCubic([](double t) { return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; });
	const TransitionFunction easeInExpo([](double t) { return t == 0 ? 0 : pow(2, 10 * (t - 1)); });
	const TransitionFunction easeOutExpo([](double t) { return t == 1 ? 1 : 1 - pow(2, -10 * t); });
	const TransitionFunction easeInOutExpo([](double t) { return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? pow(2, 20 * t - 10) / 2 : (2 - pow(2, -20 * t + 10)) / 2; });

	class Transition
	{
	public:
		Transition(double _duration, const TransitionFunction& _function = linear);
		Transition(double _duration, TransitionFunction&& _function);
		Transition(double _duration, const std::function<double(double)>& _function);
		double getValue(double beginValue, double endValue, double elapsedTime) const;
	private:
		double duration_;
		std::shared_ptr<TransitionFunction> basicFunction_;
	};
}