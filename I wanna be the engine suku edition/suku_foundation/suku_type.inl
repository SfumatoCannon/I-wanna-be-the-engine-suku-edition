#pragma once

namespace suku
{
	template<typename BaseObjectT>
	template<typename T> requires std::is_base_of_v<BaseObjectT, T>
	void SukuObjectTypeTree<BaseObjectT>::append()
	{
		if (typeMap_[typecode(T)] == nullptr)
		{
			TypeNode* newNode = new TypeNode;
			newNode->code = typecode(T);
			newNode->father = nullptr;
			typeMap_[typecode(T)] = newNode;
			convertCheckMap_[typecode(T)] = [](BaseObjectT* _o)->bool {
				T* result = dynamic_cast<T*>(_o);
				return result != nullptr;
				};
		}
	}

	template<typename BaseObjectT>
	template<typename T> requires std::is_base_of_v<BaseObjectT, T>
	std::list<Typecode> SukuObjectTypeTree<BaseObjectT>::getImmediateChildrenList()
	{
		TypeNode* targetNode = typeMap_[typecode(T)];
		std::list<Typecode> resultList;
		if (targetNode != nullptr)
		{
			for (auto& childNode : targetNode->child)
				resultList.push_back(childNode->code);
			return resultList;
		}
		else
			return resultList;
	}

	template<typename BaseObjectT>
	template<typename T> requires std::is_base_of_v<BaseObjectT, T>
	std::list<Typecode> SukuObjectTypeTree<BaseObjectT>::getAllChildrenList()
	{
		TypeNode* rootNode = typeMap_[typecode(T)];
		std::list<Typecode> resultList;
		if (rootNode == nullptr)
			return resultList;

		std::list<TypeNode*> searchList;
		std::map<Typecode, bool> checkMap;
		searchList.push_back(rootNode);
		resultList.push_back(rootNode->code);
		checkMap[rootNode->code] = true;
		while (!searchList.empty())
		{
			auto currentNodeIterator = searchList.begin();
			TypeNode* currentNode = *currentNodeIterator;
			for (auto& sonNode : currentNode->child)
				if (checkMap.find(sonNode->code) == checkMap.end())
				{
					searchList.push_back(sonNode);
					resultList.push_back(sonNode->code);
					checkMap[sonNode->code] = true;
				}
			searchList.erase(currentNodeIterator);
		}
		return resultList;
	}
}