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
		static String getHashedString(const String& _str);
	};

	class FileCodec
	{
	public:
		static bool writeResource(String _originalFilePath);
		static bool readResource(std::vector<char>& _byteData, String _originalFilePath);
	};
}