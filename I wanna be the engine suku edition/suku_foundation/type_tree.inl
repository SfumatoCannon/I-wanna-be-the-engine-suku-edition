#pragma once

namespace suku
{
	template<typename T> void SukuObjectTypeTree::append()
	{
		if (typeMap_[typecode(T)] == nullptr)
		{
			TypeNode* newNode = new TypeNode;
			newNode->code = typecode(T);
			newNode->father = nullptr;
			typeMap_[typecode(T)] = newNode;
		}
	}

	template<typename Father, typename Son> void SukuObjectTypeTree::link()
	{
		TypeNode* fatherNode = typeMap_[typecode(Father)];
		TypeNode* sonNode = typeMap_[typecode(Son)];
		if (fatherNode == nullptr)
		{
			append<Father>();
			fatherNode = typeMap_[typecode(Father)];
		}
		if (sonNode == nullptr)
		{
			append<Son>();
			sonNode = typeMap_[typecode(Son)];
		}
		this->unlink<Son>();
		sonNode->father = fatherNode;
		fatherNode->child.push_back(sonNode);
	}

	template<typename Son> void SukuObjectTypeTree::unlink()
	{
		TypeNode* sonNode = typeMap_[typecode(Son)];
		if (sonNode == nullptr)
			return;
		TypeNode* fatherNode = sonNode->father;
		if (fatherNode == nullptr)
			return;
		for (auto iter = fatherNode->child.begin(); iter != fatherNode->child.end(); iter++)
			if (*iter == sonNode)
			{
				fatherNode->child.erase(iter);
				break;
			}
		sonNode->father = nullptr;
	}

	template<typename T> std::list<Typecode> SukuObjectTypeTree::getImmediateChildrenList()
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

	template<typename T> std::list<Typecode> SukuObjectTypeTree::getAllChildrenList()
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