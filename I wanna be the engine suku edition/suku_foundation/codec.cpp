#include "pch.h"
#include "codec.h"
#include "string.h"
#include <sstream>
#include <iomanip>
#include "file.h"
#include "message.h"

namespace suku
{
    constexpr unsigned int seed = 31;

    void Codec::encodeData(std::vector<char>& _data)
    {
        unsigned int key = seed;
        for (auto& c : _data)
        {
            key = key * 17 + 33;
            c = c ^ static_cast<char>(key & 255);
        }
    }

    void Codec::decodeData(std::vector<char>& _data)
    {
        unsigned int key = seed;
        for (auto& c : _data)
        {
            key = key * 17 + 33;
            c = c ^ static_cast<char>(key & 255);
        }
    }

    String Codec::getHashedString(const String& _str)
    {
        uint64_t hash = 1469598103934665603ULL;

        for (unsigned char c : _str.contentInString())
        {
            hash ^= c;
            hash *= 1099511628211ULL;
        }

        std::ostringstream oss;
        oss << std::hex << std::setw(16) << std::setfill('0') << hash;
        return String(oss.str());
    }

    bool FileCodec::writeResource(String _originalFilePath)
    {
        String absoluteFilePath = filesystem::absolutePath(_originalFilePath);
        File originalFile(absoluteFilePath);
        std::vector<char> fileData;
        if (originalFile.tryOpenForRead() == false)
        {
            WARNINGWINDOW_GLOBAL("Failed to open file: " + _originalFilePath);
            return false;
        }
        originalFile.read(fileData);
        originalFile.closeRead();
        Codec::encodeData(fileData);
        File encodedFile(filesystem::absolutePath("GameAssets\\" + Codec::getHashedString(_originalFilePath) + ".dat"));
        if (encodedFile.tryOpenForWrite() == false)
        {
            WARNINGWINDOW_GLOBAL("Failed to open converted resource file: " + _originalFilePath);
            return false;
        }
        else
        {
            encodedFile.write(fileData);
            encodedFile.closeWrite();
            return true;
        }
    }

    bool FileCodec::readResource(std::vector<char>& _byteData, String _originalFilePath)
    {
        File encodedFile(filesystem::absolutePath("GameAssets\\" + Codec::getHashedString(_originalFilePath) + ".dat"));
        if (encodedFile.tryOpenForRead() == false)
        {
            ERRORWINDOW_GLOBAL("No file or resource file found: " + String(_originalFilePath));
            return false;
        }
        encodedFile.read(_byteData);
        encodedFile.closeRead();
        Codec::decodeData(_byteData);
        return true;
    }
}