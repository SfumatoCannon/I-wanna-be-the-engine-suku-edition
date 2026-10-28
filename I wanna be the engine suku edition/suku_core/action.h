#pragma once
#include "room.h"
#include "suku_core_concepts.h"
#include <type_traits>
#include <suku_foundation/suku_type.h>
#include <functional>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define REGISTERED_ACTION_NAME CONCAT(action_registered_, __COUNTER__)

#define ACTION(func) inline suku::ActionRegisterClass<&(func)> REGISTERED_ACTION_NAME {};

#define ACTION_ORDER(func, func2) inline suku::ActionOrderRegisterClass<&(func), &(func2)> REGISTERED_ACTION_NAME {};

namespace suku
{
	class IAction
	{
	public:
		virtual ~IAction() = default;
		virtual void execute(Room* _inRoom) = 0;
	protected:
		unsigned int allocateId() { idCounter_++; return idCounter_; }
	private:
		inline static unsigned int idCounter_ = 0;
	};

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	class Action : public IAction
	{
	public:
		Action();
		virtual void execute(Room* _inRoom) override { executeFunction_(_inRoom); }
		unsigned int getId() const { return id_; }
	private:
		unsigned int registerNewId();
		std::function<void(Room*)> executeFunction_;
		unsigned int id_;
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
		void registerAction();

		template<auto Func>
			requires suku_upd_function<decltype(Func)>
		ActionNode* findActionNode();

		template<auto FuncSrc, auto FuncDst>
			requires suku_upd_function<decltype(FuncSrc)> && suku_upd_function<decltype(FuncDst)>
		void addDependency();

		void execute(Room* _inRoom);

		static ActionManager& getInstance()
		{
			static ActionManager instance;
			return instance;
		}
	private:
		ActionManager() = default;
		std::map<Typecode, ActionNode> actionNodePool_;
	};

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	class ActionRegisterClass
	{
	public:
		ActionRegisterClass() { ActionManager::getInstance().registerAction<Func>(); }
	};

	template<auto Func1, auto Func2>
		requires suku_upd_function<decltype(Func1)> && suku_upd_function<decltype(Func2)>
	class ActionOrderRegisterClass
	{
	public:
		ActionOrderRegisterClass() { ActionManager::getInstance().addDependency<Func1, Func2>(); }
	};
}

#include "action.inl"