#include "action.h"
#include <list>

namespace suku
{
	template<auto Func>
		requires std::is_member_function_pointer_v<decltype(Func)>&& suku_object<suku_upd_function_source_class_t<decltype(Func)>>
	inline Action<Func>::Action() : id_(typecode(Func))
	{
		using Obj = suku_upd_function_source_class_t<decltype(Func)>;
		executeFunction_ = [](Room* _inRoom) {
			std::list<Obj*> objList = _inRoom->getObjectList<Obj>();
			for (auto& op : objList)
			{
				(op->*Func)();
			}
			};
	}
}