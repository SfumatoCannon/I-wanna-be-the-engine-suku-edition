#pragma once
#include <list>
#include <map>
#include <functional>

#define typecode(T) typeid(T).hash_code()
//#define SUKU_DERIVE(Derived, Base) \
//	static inline TypeRegisterInheritance<Derived, Base> __suku_type_register_inheritance_instance_##Derived##_##Base {};

namespace suku
{
	typedef size_t Typecode;
	// alias
		typedef Typecode typecode_t;

	class TypeNode
	{
	public:
		Typecode code = 0;
		std::list<TypeNode*> child;
		TypeNode* father = nullptr;
	};

	template<typename BaseObjectT>
	class SukuObjectTypeTree
	{
	public:
		static SukuObjectTypeTree<BaseObjectT>& getInstance()
		{
			static SukuObjectTypeTree<BaseObjectT> instance;
			return instance;
		}

		SukuObjectTypeTree(const SukuObjectTypeTree&) = delete;
		SukuObjectTypeTree& operator=(const SukuObjectTypeTree&) = delete;

		template<typename T> 
		requires std::is_base_of_v<BaseObjectT, T>
		void append();

		template<typename T>
			requires std::is_base_of_v<BaseObjectT, T>
		std::list<Typecode> getImmediateChildrenList();

		template<typename T>
			requires std::is_base_of_v<BaseObjectT, T>
		std::list<Typecode> getAllChildrenList();

	private:
		std::map<Typecode, std::function<bool(BaseObjectT*)>> convertCheckMap_;
		std::map<Typecode, TypeNode*> typeMap_;
		SukuObjectTypeTree() {}
	};

	//template<typename Derived, typename Base>
	//struct TypeRegisterInheritance 
	//{
	//	TypeRegisterInheritance()
	//	{
	//		SukuObjectTypeTree::getInstance().link<Base, Derived>();
	//	}
	//};
}

#include "suku_type.inl"