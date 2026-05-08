#include "pch.h"
#include "transition.h"

namespace suku
{
	Transition::Transition(double _duration, const TransitionFunction& _function)
		: duration_(_duration), basicFunction_(_function)
	{}

	Transition::Transition(double _duration, TransitionFunction&& _function)
		: duration_(_duration), basicFunction_(_function)
	{}

	Transition::Transition(double _duration, const std::function<double(double)>&_function)
		: duration_(_duration), basicFunction_(TransitionFunction(_function))
	{}

	double Transition::getValue(double beginValue, double endValue, double elapsedTime) const
	{
		double x = std::clamp(elapsedTime / duration_, 0.0, 1.0);
		return beginValue + (endValue - beginValue) * basicFunction_.getValue(x);
	}
}