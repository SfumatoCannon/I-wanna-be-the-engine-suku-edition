#pragma once
#include "suku_core_concepts.h"

namespace suku
{
	typedef double execute_priority_t;
	class ExecutePriority
	{
		template<suku_object Obj>
		static execute_priority_t before();

		template<suku_object Obj>
		static execute_priority_t after();

		template<suku_object Obj1, suku_object Obj2>
		static execute_priority_t between();
	};
}