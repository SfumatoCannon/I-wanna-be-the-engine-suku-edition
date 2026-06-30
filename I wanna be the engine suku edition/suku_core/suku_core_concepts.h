#pragma once
#include <concepts>
namespace suku
{
	class Room;
	class Object;

	template<typename T>
	concept suku_room = std::derived_from<T, Room>;

	template<typename T>
	concept suku_object = std::is_base_of_v<Object, T>;

	template<typename T>
	concept suku_property_type = std::is_arithmetic<T>::value;
}