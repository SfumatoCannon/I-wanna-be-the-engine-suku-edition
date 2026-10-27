#include <list>
#include "room.h"

namespace suku
{
	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	inline Action<Func>::Action() : id_(typecode(Func))
	{
		using Obj = suku_upd_function_source_class_t<decltype(Func)>;
		executeFunction_ = [](Room* _inRoom) {
			std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
			for (Obj* o : objList)
			{
				if (o->isExist() && !o->isFrozen())
				{
					(o->*Func)();
				}
			}
		};
	}


	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	inline void ActionManager::registerAction()
	{
		actionNodePool_.try_emplace(typecode(Func), ActionNode{ std::make_unique<Action<Func>>() });
	}

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	inline ActionNode* ActionManager::findActionNode()
	{
		auto iter = actionNodePool_.find(typecode(Func));
		if (iter != actionNodePool_.end())
			return &(iter->second);
		else
			return nullptr;
	}

	template<auto FuncSrc, auto FuncDst>
		requires suku_upd_function<decltype(FuncSrc)> && suku_upd_function<decltype(FuncDst)>
	inline void ActionManager::addDependency()
	{
		registerAction<FuncSrc>();
		registerAction<FuncDst>();
		ActionNode* srcNode = findActionNode<FuncSrc>();
		ActionNode* dstNode = findActionNode<FuncDst>();
		if (srcNode == nullptr || dstNode == nullptr)
		{
			return;
		}
		bool flag = false;
		for (auto& i : srcNode->next)
		{
			if (i == dstNode)
			{
				flag = true;
				break;
			}
		}
		if (!flag)
		{
			srcNode->next.push_back(dstNode);
			dstNode->prev.push_back(srcNode);
		}
	}
}