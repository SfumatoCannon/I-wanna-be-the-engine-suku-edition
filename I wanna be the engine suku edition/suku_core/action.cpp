#include "action.h"

namespace suku
{
	void ActionManager::execute(Room* _inRoom)
	{
		for (auto& node : actionExecutionQueue_)
		{
			node->action->execute(_inRoom);
		}
	}
}