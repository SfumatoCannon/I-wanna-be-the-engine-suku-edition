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

		static const TransitionCurve linear;
		static const TransitionCurve easeInQuad;
		static const TransitionCurve easeOutQuad;
		static const TransitionCurve easeInOutQuad;
		static const TransitionCurve easeInCubic;
		static const TransitionCurve easeOutCubic;
		static const TransitionCurve easeInOutCubic;
		static const TransitionCurve easeInExpo;
		static const TransitionCurve easeOutExpo;
		static const TransitionCurve easeInOutExpo;
	private:
		std::function<double(double)> function_;
	};

	class Transition
	{
	public:
		Transition(double _duration, const TransitionCurve& _curve = TransitionCurve::linear);
		Transition(double _duration, TransitionCurve&& _curve);
		Transition(double _duration, const std::function<double(double)>& _curveFunction);
		double getValue(double beginValue, double endValue, double elapsedTime) const;
	private:
		double duration_;
		const TransitionCurve& curve_;
	};
}