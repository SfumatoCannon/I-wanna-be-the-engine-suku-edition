#include "action.h"

namespace suku
{
	void ActionManager::execute(Room* _inRoom)
	{
		std::list<ActionNode*> remainNodes;
		for (auto& [id, node] : actionNodePool_)
		{
			node.prevCounter = node.prev.size();
			remainNodes.push_back(&node);
		}
		while (!remainNodes.empty())
		{
			for (auto iter = remainNodes.begin(); iter != remainNodes.end();)
			{
				ActionNode* node = *iter;
				if (node->prevCounter == 0)
				{
					node->action->execute(_inRoom);
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
		}
	}
}