#pragma once
#include <list>
#include <map>

#define typecode(T) typeid(T).hash_code()
#define SUKU_DERIVE(Derived, Base) \
	static inline TypeRegisterInheritance<Derived, Base> __suku_type_register_inheritance_instance_##Derived##_##Base {};
namespace suku
{
	typedef size_t Typecode;

	class Vector;
	class TypeNode;
	class SukuObjectTypeTree;

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
		Typecode code = 0;
		std::list<TypeNode*> child;
		TypeNode* father = nullptr;
	};

	class SukuObjectTypeTree
	{
	public:
		static SukuObjectTypeTree& getInstance()
		{
			static SukuObjectTypeTree instance;
			return instance;
		}

		SukuObjectTypeTree(const SukuObjectTypeTree&) = delete;
		SukuObjectTypeTree& operator=(const SukuObjectTypeTree&) = delete;

		template<typename T> void append();
		template<typename Father, typename Son> void link();
		template<typename Son> void unlink();
		template<typename T> std::list<Typecode> getImmediateChildrenList();
		template<typename T> std::list<Typecode> getAllChildrenList();

	private:
		std::map<Typecode, TypeNode*> typeMap_;
		SukuObjectTypeTree() {}
	};

	template<typename Derived, typename Base>
	struct TypeRegisterInheritance 
	{
		TypeRegisterInheritance()
		{
			SukuObjectTypeTree::getInstance().link<Base, Derived>();
		}
	};
}

#include "type_tree.inl"