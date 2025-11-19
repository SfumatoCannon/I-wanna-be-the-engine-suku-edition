#pragma once
#include <list>
#include <map>

#define typecode(T) typeid(T).hash_code()
namespace suku
{

	typedef size_t Typecode;

	class Vector
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		Vector(float _x, float _y) : x(_x), y(_y) {}
		Vector(double _x, double _y) : x((float)_x), y((float)_y) {}
	};

	class TypeNode
	{
	public:
		size_t code = 0;
		std::list<TypeNode*> child;
		TypeNode* father = nullptr;
	};

	class TypeTree
	{
	private:
		std::map<size_t, TypeNode*> typeMap_;
	public:
		template<typename T> void append();

		template<typename Father, typename Son> void link();

		template<typename Son> void unlink();

		template<typename T> std::list<TypeNode*> getImmediateChildrenList();

		template<typename T> std::list<TypeNode*> getAllChildrenList();
	};
}

#include "type_tree.inl"