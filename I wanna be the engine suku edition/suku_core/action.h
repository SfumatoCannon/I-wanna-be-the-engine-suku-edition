#pragma once
#include "room.h"
#include "suku_core_concepts.h"
#include <type_traits>
#include <suku_foundation/suku_type.h>
#include <functional>

namespace suku
{
	class IAction
	{
	public:
		virtual ~IAction() = default;
		virtual void execute(Room* _inRoom) = 0;
	};

	template<auto Func>
		requires std::is_member_function_pointer_v<decltype(Func)>&& suku_object<suku_upd_function_source_class_t<decltype(Func)>>
	class Action : public IAction
	{
	public:
		Action();
		virtual void execute(Room* _inRoom) override { executeFunction_(_inRoom); }
		Typecode getId() { return id_; }
	private:
		Typecode id_;
		std::function<void(Room*)> executeFunction_;
	};

	struct ActionNode
	{
		std::unique_ptr<IAction> action;
		std::list<ActionNode*> prev;
		std::list<ActionNode*> next;
		int prevCounter = 0;
	};

	class ActionManager
	{
	private:
		std::map<long double, ActionNode> actionNodePool_;
	};
}

#include "action.inl"