#pragma once
#include <concepts>
#include <suku_foundation/maths.h>
namespace suku
{
	class Room;
	class Object;

	template<typename T>
	concept suku_room = std::derived_from<T, Room>;

	template<typename T>
	concept suku_object = std::is_base_of_v<Object, T>;

	template<typename T>
	concept suku_property_type = std::is_arithmetic<T>::value || std::is_same_v<T, Real>;

	template<typename T>
	struct suku_upd_function_source_class;

	template<typename Obj>
	struct suku_upd_function_source_class<void(Obj::*)()>
	{
		using type = Obj;
	};

	template<typename Obj>
	struct suku_upd_function_source_class<void(Obj::*)() const>
	{
		using type = Obj;
	};

	template<typename T>
	using suku_upd_function_source_class_t =
		typename suku_upd_function_source_class<T>::type;
}