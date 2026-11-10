#pragma once
#include "room.h"
#include "suku_core_concepts.h"
#include <type_traits>
#include <suku_foundation/suku_type.h>
#include <functional>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define REGISTERED_ACTION_NAME CONCAT(action_registered_, __COUNTER__)

#define ACTION(func) inline suku::UpdateActionRegisterClass<&(func)> REGISTERED_ACTION_NAME {};
#define ACTION_ORDER(func, func2) inline suku::UpdateActionOrderRegisterClass<&(func), &(func2)> REGISTERED_ACTION_NAME {};

#define PAINT_ACTION(func) inline suku::PaintActionRegisterClass<&(func)> REGISTERED_ACTION_NAME {};
#define PAINT_ACTION_ORDER(func, func2) inline suku::PaintActionOrderRegisterClass<&(func), &(func2)> REGISTERED_ACTION_NAME {};

#define OBJ_UPD_ORDER(obj, obj2) inline suku::UpdateActionOrderRegisterClass<static_cast<void (obj::*)()>(&obj::onUpdate), static_cast<void (obj2::*)()>(&obj2::onUpdate)> REGISTERED_ACTION_NAME {};
#define OBJ_PAINT_ORDER(obj, obj2) inline suku::PaintActionOrderRegisterClass<static_cast<void (obj::*)()>(&obj::onPaint), static_cast<void (obj2::*)()>(&obj2::onPaint)> REGISTERED_ACTION_NAME {};

namespace suku
{
	enum class ActionType
	{
		Update,
		Paint
	};

	class IAction
	{
	public:
		virtual ~IAction() = default;
		virtual void execute(Room* _inRoom) = 0;
		virtual void executeWithOffset(Room* _inRoom, float _frameOffsetRate) = 0;
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
		Action(ActionType _type);
		virtual void execute(Room* _inRoom) override { executeFunction_(_inRoom); }
		virtual void executeWithOffset(Room* _inRoom, float _frameOffsetRate) override { executeFunctionWithOffset_(_inRoom, _frameOffsetRate); }
		unsigned int getId() const { return id_; }
	private:
		unsigned int registerNewId();
		std::function<void(Room*)> executeFunction_;
		std::function<void(Room*, float)> executeFunctionWithOffset_;
		unsigned int id_;
	};

	struct ActionNode
	{
		std::unique_ptr<IAction> action;
		std::list<ActionNode*> prev;
		std::list<ActionNode*> next;
		size_t prevCounter = 0;
		ActionNode(std::unique_ptr<IAction> _action) : action(std::move(_action)) {}
	};

	class ActionManager
	{
	public:
		ActionType type = ActionType::Update;

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
		void executeWithOffset(Room* _inRoom, float _frameOffsetRate);

		static ActionManager& getUpdInstance()
		{
			static ActionManager instance;
			instance.type = ActionType::Update;
			return instance;
		}

		static ActionManager& getPntInstance()
		{
			static ActionManager instance;
			instance.type = ActionType::Paint;
			return instance;
		}
	private:
		ActionManager() = default;
		std::map<unsigned int, ActionNode> actionNodePool_;
		std::list<ActionNode*> actionExecutionQueue_;
	};

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	class UpdateActionRegisterClass
	{
	public:
		UpdateActionRegisterClass() { ActionManager::getUpdInstance().registerAction<Func>(); }
	};

	template<auto Func1, auto Func2>
		requires suku_upd_function<decltype(Func1)> && suku_upd_function<decltype(Func2)>
	class UpdateActionOrderRegisterClass
	{
	public:
		UpdateActionOrderRegisterClass() { ActionManager::getUpdInstance().addDependency<Func1, Func2>(); }
	};

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	class PaintActionRegisterClass
	{
	public:
		PaintActionRegisterClass() { ActionManager::getPntInstance().registerAction<Func>(); }
	};

	template<auto Func1, auto Func2>
		requires suku_upd_function<decltype(Func1)>&& suku_upd_function<decltype(Func2)>
	class PaintActionOrderRegisterClass
	{
	public:
		PaintActionOrderRegisterClass() { ActionManager::getPntInstance().addDependency<Func1, Func2>(); }
	};
}

#include "action.inl"