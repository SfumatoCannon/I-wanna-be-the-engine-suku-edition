#pragma once
#include "room.h"
#include "suku_core_concepts.h"
#include <type_traits>
#include <suku_foundation/suku_type.h>
#include <functional>

namespace suku
{
	class Action
	{
	public:
		template<auto Func>
			requires std::is_member_function_pointer_v<decltype(Func)> && suku_object<suku_upd_function_source_class<decltype(Func)>>
		Action();
	private:
		Typecode id_;
		std::function<void(Room*)> executeFunction_;
	};
}

#include "action.inl"