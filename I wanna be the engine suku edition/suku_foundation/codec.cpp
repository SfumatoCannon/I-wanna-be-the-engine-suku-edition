#include "codec.h"
#include "string.h"
#include <sstream>
#include <iomanip>

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
}