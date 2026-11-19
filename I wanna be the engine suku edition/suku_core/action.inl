#include <list>

namespace suku
{
	template<auto Func, action_tag Tag>
		requires suku_upd_function<decltype(Func)>
	inline Action<Func, Tag>::Action() : id_(registerNewId())
	{
		using Obj = suku_upd_function_source_class_t<decltype(Func)>;
		bool overrideCheck = false;
		std::map<Typecode, bool>& overrideCheckMap = IAction::hasOverridedOnUpdateStart;
		if constexpr (IAction::isOnUpdateStartFunc<Func>())
		{
			overrideCheck = true;
			overrideCheckMap = IAction::hasOverridedOnUpdateStart;
		}
		else if constexpr (IAction::isOnUpdateFunc<Func>())
		{
			overrideCheck = true;
			overrideCheckMap = IAction::hasOverridedOnUpdate;
		}
		else if constexpr (IAction::isOnUpdateEndFunc<Func>())
		{
			overrideCheck = true;
			overrideCheckMap = IAction::hasOverridedOnUpdateEnd;
		}

		if constexpr (std::is_same_v<Obj, Object>)
		{
			executeFunction_ = [overrideCheck, &overrideCheckMap](Room* _inRoom) {
				std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
				for (Obj* o : objList)
				{
					if (overrideCheck)
					{
						if (overrideCheckMap[o->getTypecode()] == true)
							continue;
					}
					bool flag;
					if constexpr (std::is_same_v<Tag, UpdateActionTag>)
						flag = (o->isExist() && !o->isFrozen());
					else // is_same_v<Tag, PaintActionTag>
						flag = (o->isExist() && o->isVisible());
					if (flag)
					{
						(o->*Func)();
					}
				}
				};
			executeFunctionWithOffset_ = [overrideCheck, &overrideCheckMap](Room* _inRoom, float _frameOffsetRate) {
				std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
				for (Obj* o : objList)
				{
					if (overrideCheck)
					{
						if (overrideCheckMap[o->getTypecode()] == true)
							continue;
					}
					bool flag;
					if constexpr (std::is_same_v<Tag, UpdateActionTag>)
						flag = (o->isExist() && !o->isFrozen());
					else // is_same_v<Tag, PaintActionTag>
						flag = (o->isExist() && o->isVisible());
					if (flag)
					{
						o->applyTransactions_(_frameOffsetRate);

						(o->*Func)();

						o->removeTransactions_();
					}
				}
				};
		}
		else // !std::is_same_v<Obj, Object>
		{
			if (overrideCheck)
			{
				overrideCheckMap[typecode(Obj)] = true;
			}
			executeFunction_ = [](Room* _inRoom) {
				std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
				for (Obj* o : objList)
				{
					bool flag;
					if constexpr (std::is_same_v<Tag, UpdateActionTag>)
						flag = (o->isExist() && !o->isFrozen());
					else // is_same_v<Tag, PaintActionTag>
						flag = (o->isExist() && o->isVisible());
					if (flag)
					{
						(o->*Func)();
					}
				}
				};
			executeFunctionWithOffset_ = [=](Room* _inRoom, float _frameOffsetRate) {
				std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
				for (Obj* o : objList)
				{
					bool flag;
					if constexpr (std::is_same_v<Tag, UpdateActionTag>)
						flag = (o->isExist() && !o->isFrozen());
					else // is_same_v<Tag, PaintActionTag>
						flag = (o->isExist() && o->isVisible());
					if (flag)
					{
						o->applyTransactions_(_frameOffsetRate);

						(o->*Func)();

						o->removeTransactions_();
					}
				}
				};
		}
	}

	template<auto Func, action_tag Tag>
		requires suku_upd_function<decltype(Func)>
	inline unsigned int Action<Func, Tag>::registerNewId()
	{
		static unsigned int newId = IAction::allocateId();
		return newId;
	}

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	inline void ActionManager::registerAction()
	{
		if (type == ActionType::Update)
		{
			auto pFunc = std::make_unique<Action<Func, UpdateActionTag>>();
			const auto id = pFunc->getId();
			auto result = actionNodePool_.try_emplace(id, ActionNode{ std::move(pFunc) });
			if (result.second == true)
			{
				actionExecutionQueue_.push_back(&(result.first->second));
			}
		}
		else // type == ActionType::Paint
		{
			auto pFunc = std::make_unique<Action<Func, PaintActionTag>>();
			const auto id = pFunc->getId();
			auto result = actionNodePool_.try_emplace(id, ActionNode{ std::move(pFunc) });
			if (result.second == true)
			{
				actionExecutionQueue_.push_back(&(result.first->second));
			}
		}
	}

	template<auto Func>
		requires suku_upd_function<decltype(Func)>
	inline ActionNode* ActionManager::findActionNode()
	{
		if (type == ActionType::Update)
		{
			auto pFunc = std::make_unique<Action<Func, UpdateActionTag>>();
			auto iter = actionNodePool_.find(pFunc->getId());
			if (iter != actionNodePool_.end())
				return &(iter->second);
			else
				return nullptr;
		}
		else // type == ActionType::Paint
		{
			auto pFunc = std::make_unique<Action<Func, PaintActionTag>>();
			auto iter = actionNodePool_.find(pFunc->getId());
			if (iter != actionNodePool_.end())
				return &(iter->second);
			else
				return nullptr;
		}
	}

	template<auto FuncSrc, auto FuncDst>
		requires suku_upd_function<decltype(FuncSrc)>&& suku_upd_function<decltype(FuncDst)>
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
		std::list<ActionNode*> queue;
		std::list<ActionNode*> remainNodes;
		for (auto& [id, node] : actionNodePool_)
		{
			node.prevCounter = node.prev.size();
			remainNodes.push_back(&node);
		}
		while (!remainNodes.empty())
		{
			bool flag = false;
			for (auto iter = remainNodes.begin(); iter != remainNodes.end();)
			{
				ActionNode* node = *iter;
				if (node->prevCounter == 0)
				{
					flag = true;
					queue.push_back(node);
					for (auto& nextNode : node->next)
					{
						nextNode->prevCounter--;
					}
					iter = remainNodes.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			if (!flag)
			{
				ERRORWINDOW(std::format("Failed to define order of {} -> {}: This will cause a cycle.", typeid(FuncSrc).name(), typeid(FuncDst).name()));
				srcNode->next.remove_if([=](ActionNode* _node) {return _node == dstNode; });
				dstNode->prev.remove_if([=](ActionNode* _node) {return _node == srcNode; });
				return;
			}
		}
		actionExecutionQueue_ = queue;
	}
}