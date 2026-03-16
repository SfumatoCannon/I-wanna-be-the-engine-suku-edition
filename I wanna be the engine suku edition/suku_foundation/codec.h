#pragma once
#include <vector>

namespace suku
{	
	class String;

	class Codec
	{
	public:
		static void encodeData(std::vector<char>& _data);
		static void decodeData(std::vector<char>& _data);
	};
}