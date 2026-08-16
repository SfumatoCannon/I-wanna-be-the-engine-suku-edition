#include "map_loader.h"
#include <suku_core/includes.h>
#include "includes.h"
#include <suku_constants.h>
// Private functions declaration
// ----------------------------------------------------------------------------
namespace
{
	int base32(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';

		if (c >= 'a' && c <= 'v')
			return c - 'a' + 10;

		throw std::runtime_error("Invalid base32 character");
	}

	int base32(std::string_view s)
	{
		int result = 0;

		for (char c : s)
			result = result * 32 + base32(c);

		return result;
	}

	struct JtoolObjectMsg
	{
		int x;
		int y;
		int type;
	};
}
// ----------------------------------------------------------------------------
// End of private functions declaration

namespace suku
{
	std::vector<JtoolObjectMsg> decodeJtoolObjects(std::string_view _str)
	{
		std::vector<JtoolObjectMsg> result;

		int y = 0;
		size_t i = 0;

		while (i < _str.size())
		{
			if (_str[i] == '-')
			{
				y = base32(_str.substr(i + 1, 2)) - 128;
				i += 3;
			}
			else
			{
				int type = base32(_str.substr(i, 1));
				int x = base32(_str.substr(i + 1, 2)) - 128;

				result.push_back({
					x,
					y,
					type
					});
				i += 3;
			}
		}

		return result;
	}

	void MapLoader::loadFromJtool(suku::Room* _room, std::string_view _msg)
	{
		using namespace suku;
		std::vector<JtoolObjectMsg> objectMsgList = decodeJtoolObjects(_msg);
		for (auto& i : objectMsgList)
		{
			switch (i.type)
			{
			case 1:
				_room->create<Wall>((float)i.x, (float)i.y);
				break;
			case 3:
				_room->create<Spike>((float)i.x, (float)i.y, Direction::Up);
				break;
			case 4:
				_room->create<Spike>((float)i.x, (float)i.y, Direction::Right);
				break;
			case 5:
				_room->create<Spike>((float)i.x, (float)i.y, Direction::Left);
				break;
			case 6:
				_room->create<Spike>((float)i.x, (float)i.y, Direction::Down);
				break;
			case 20:
				_room->create<Player>((float)i.x, (float)i.y);
				break;
			default:
				break;
			}
		}
	}
}