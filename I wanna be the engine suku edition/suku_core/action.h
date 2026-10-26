#pragma once
#include "room.h"
#include "suku_core_concepts.h"
#include <type_traits>
#include <suku_foundation/suku_type.h>
#include <functional>

#define ACTION(func) static inline suku::ActionRegisterClass<func> action_registered_##func## {};

#define ACTION_ORDER(func, func2) static inline suku::ActionOrderRegisterClass<func, func2> action_order_registered_##func##_##func2## {};

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

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	class ActionRegisterClass
	{
	public:
		ActionRegisterClass() { ActionManager::registerAction<Func>(); }
	};

	template<auto Func1, auto Func2>
		requires suku_upd_function<decltype(Func1)> && suku_upd_function<decltype(Func2)>
	class ActionOrderRegisterClass
	{
	public:
		ActionOrderRegisterClass() { ActionManager::addDependency<Func1, Func2>(); }
	};
}

#include "action.inl"