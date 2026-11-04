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

	void ActionManager::executeWithOffset(Room* _inRoom, float _frameOffsetRate)
	{
		for (auto& node : actionExecutionQueue_)
		{
			node->action->executeWithOffset(_inRoom, _frameOffsetRate);
		}
	}
}