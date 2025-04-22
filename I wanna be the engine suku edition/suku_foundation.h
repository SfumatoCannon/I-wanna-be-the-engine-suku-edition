#pragma once
#include "framework.h"

namespace suku
{
	//----------------------------------------------
	//           Error Handling Section
	//----------------------------------------------
#define ERRORWINDOW(message) MessageBoxExW(NULL, \
	String(String("In function: ") + typeid(this).name() + "." + __func__ + "\n" + message).content, \
	L"Error", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))



	//----------------------------------------------
	//           Player Input Section
	//----------------------------------------------
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

	extern bool isKeyDown[256];
	extern bool isKeyHolding[256];
	extern bool isKeyUp[256];
	extern std::queue<std::pair<UINT, WPARAM> > keyMsg;
	void pushKeyMessage(UINT _message, WPARAM _wParam);
	void keyCheck();
	void resetKey();




	//----------------------------------------------
	//			  Type Tree Section
	//----------------------------------------------
#define typecode(T) typeid(T).hash_code()

	//typedef double real;
	//typedef std::pair<real, real> pair;
	typedef std::pair<float, float> Vector;

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
		template<typename T> void append()
		{
			if (typeMap_[typecode(T)] == nullptr)
			{
				TypeNode* newNode = new TypeNode;
				newNode->code = typecode(T);
				newNode->father = nullptr;
				typeMap_[typecode(T)] = newNode;
			}
		}

		template<typename Father, typename Son> void link()
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

		template<typename Son> void unlink()
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

		template<typename T> std::list<TypeNode*> getChildList()
		{
			TypeNode* targetNode = typeMap_[typecode(T)];
			if (targetNode != nullptr)
				return targetNode->child;
			else
				return std::list<TypeNode*>();
		}

		template<typename T> std::list<TypeNode*> getAllChildList()
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
	};




	//----------------------------------------------
	//			  Memory Section
	//----------------------------------------------
	template<typename T> T** malloc2D(UINT _width, UINT _height);
	template<typename T> void free2D(T** _pointer, UINT _width, UINT _height);

	template<typename T>
	inline T** malloc2D(UINT _width, UINT _height)
	{
		T** resultPointer = new T * [_height];
		if (resultPointer != nullptr)
		{
			for (UINT i = 0; i < _height; i++)
				resultPointer[i] = new T[_width];
		}
		return resultPointer;
	}

	template<typename T>
	inline void free2D(T** _pointer, UINT _width, UINT _height)
	{
		if (_pointer == nullptr)
			return;
		for (UINT i = 0; i < _height; i++)
			if (_pointer[i] != nullptr)
				delete[] _pointer[i];
		delete[] _pointer;
	}



	//----------------------------------------------
	//            String Section
	//----------------------------------------------
	class String
	{
	public:
		wchar_t* content;
		std::wstring contentInWString()const;
		std::string contentInString()const;
		String();
		String(const char* _string);
		String(std::string _string);
		String(const wchar_t* _wstring);
		String(std::wstring _wstring);
		String(const String& _other);
		void operator=(const String& _other);
		String operator+(const String& _other);
		~String();
	};
	String operator+(const char* _string1, const String& _string2);
	String operator+(const wchar_t* _string1, const String& _string2);
	String operator+(std::string _string1, const String& _string2);
	String operator+(std::wstring _string1, const String& _string2);

	wchar_t getWideChar(char _multiByteChar);
	char getMultiByteChar(wchar_t _wideChar);
	wchar_t* getWideString(const char* _multiByteString);
	char* getMultiByteString(const wchar_t* _wideString);


	//----------------------------------------------
	//			  Maths Section
	//----------------------------------------------
#define COINCIDENT_LINES 1
#define PARALLEL_LINES 2
#define	INTERSECTING_LINES 3

#define PI 3.14159265358979323846

	double bRound(double _x);
	float bRound(float _x);

	long long randL();
	int randI();
	int randI(int _left, int _right);
	double randD(double _left, double _right);
	float randF(float _left, float _right);
	template<typename T> T randInList(T _a, T _b, ...);

	int gcd(int _a, int _b);
	int lcm(int _a, int _b);

	// or use fmod() in <cmath>
	double modR(double _a, double _b);	// a % b
	// or use fmod() in <cmath>
	float modR(float _a, float _b);	// a % b

	unsigned long long hash(std::string _str);

	double radToDeg(double _radian);
	double degToRad(double _degree);

	double maxD(std::list<double> _numlist);
	double minD(std::list<double> _numlist);

	double getDistance(double _x1, double _y1, double _x2, double _y2);

	void dotToLine(double _x1, double _y1, double _x2, double _y2, double* _k, double* _b);
	int getRelationOfLines(double _k1, double _b1, double _k2, double _b2);
	void getIntersectionOfLines(double _k1, double _b1, double _k2, double _b2,
		double* _x, double* _y);
	bool isSegmentsIntersect(double _x11, double _y11, double _x12, double _y12,
		double _x21, double _y21, double _x22, double _y22);

	void rectangularToPolar(double width, double height, double* angle, double* length);
	void polarToRectangular(double angle, double length, double* width, double* height);
	void rotateDot(double rotateCenterX, double rotateCenterY, double angle, double* x, double* y);

	bool isCrashedRect(double x1, double y1, double width1, double height1,
		double x2, double y2, double width2, double height2);
	double distanceOfRect(double _x1, double _y1, double _width1, double _height1,
		double _x2, double _y2, double _width2, double _height2);

	template<typename T>
	inline T randInList(T _a, T _b, ...)
	{
		std::vector<T> vec;
		vec.push_back(_a);
		vec.push_back(_b);
		va_list argList;
		va_start(argList, _b);
		T x;
		while (1)
		{
			x = va_arg(argList, T);
			if (x == NULL)
				break;
			else vec.push_back(x);
		}
		va_end(argList);
		int id = randI() % vec.size();
		return vec[id];
	}
}
