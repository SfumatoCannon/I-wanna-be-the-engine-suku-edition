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
		requires suku_upd_function<decltype(Func)>
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
		ActionNode(std::unique_ptr<IAction> _action) : action(std::move(_action)) {}
	};

	class ActionManager
	{
	public:
		template<auto Func>
			requires suku_upd_function<decltype(Func)>
		static void registerAction();

		template<auto Func>
			requires suku_upd_function<decltype(Func)>
		static ActionNode* findActionNode();

		template<auto FuncSrc, auto FuncDst>
			requires suku_upd_function<decltype(FuncSrc)> && suku_upd_function<decltype(FuncDst)>
		static void addDependency();

		static void execute(Room* _inRoom);
	private:
		inline static std::map<Typecode, ActionNode> actionNodePool_;
	};
}

#include "action.inl"