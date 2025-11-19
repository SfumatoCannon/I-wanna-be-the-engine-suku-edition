#pragma once

namespace suku
{
	template<typename T> void TypeTree::append()
	{
		if (typeMap_[typecode(T)] == nullptr)
		{
			TypeNode* newNode = new TypeNode;
			newNode->code = typecode(T);
			newNode->father = nullptr;
			typeMap_[typecode(T)] = newNode;
		}
	}

	template<typename Father, typename Son> void TypeTree::link()
	{
		TypeNode* fatherNode = typeMap_[typecode(Father)];
		TypeNode* sonNode = typeMap_[typecode(Son)];
		if (fatherNode != nullptr && sonNode != nullptr)
		{
			this->unlink<Son>();
			sonNode->father = fatherNode;
			fatherNode->child.push_back(sonNode);
		}
	}

	template<typename Son> void TypeTree::unlink()
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

	template<typename T> std::list<TypeNode*> TypeTree::getImmediateChildrenList()
	{
		TypeNode* targetNode = typeMap_[typecode(T)];
		if (targetNode != nullptr)
			return targetNode->child;
		else
			return std::list<TypeNode*>();
	}

	template<typename T> std::list<TypeNode*> TypeTree::getAllChildrenList()
	{
		TypeNode* rootNode = typeMap_[typecode(T)];
		if (rootNode == nullptr)
			return std::list<TypeNode*>();

		std::list<TypeNode*> resultList;
		std::list<TypeNode*> searchList;
		std::map<size_t, bool> checkMap;
		searchList.push_back(rootNode);
		resultList.push_back(rootNode);
		checkMap[rootNode->code] = true;
		while (!searchList.empty())
		{
			auto currentNodeIterator = searchList.begin();
			TypeNode* currentNode = *currentNodeIterator;
			for (auto& sonNode : currentNode->child)
				if (checkMap.find(sonNode->code) == checkMap.end())
				{
					searchList.push_back(sonNode);
					resultList.push_back(sonNode);
					checkMap[sonNode->code] = true;
				}
			searchList.erase(currentNodeIterator);
		}
		return resultList;
	}
}