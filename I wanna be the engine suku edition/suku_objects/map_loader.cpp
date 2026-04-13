#include "map_loader.h"
#include <suku_core/includes.h>
#include "includes.h"
#include <global_value.h>

namespace suku
{
	void MapLoader::loadFromJtoolMsg(suku::Room* _room, std::string _msg)
	{
		using namespace suku;
		size_t length = _msg.length();
		int value[3], now = 0;//0:x  1:y  2:kind
		int temp = 0;
		for (size_t i = 0; i < length; i++)
		{
			if (_msg[i] == ' ')
			{
				value[now] = temp;
				temp = 0;
				if (now == 2)
				{
					switch (value[2])
					{
					case 1:
						_room->create(Wall((float)value[0], (float)value[1]));
						break;
					case 3:
						_room->create(Spike((float)value[0], (float)value[1], Direction::Up));
						break;
					case 4:
						_room->create(Spike((float)value[0], (float)value[1], Direction::Right));
						break;
					case 5:
						_room->create(Spike((float)value[0], (float)value[1], Direction::Left));
						break;
					case 6:
						_room->create(Spike((float)value[0], (float)value[1], Direction::Down));
						break;
					case 20:
						_room->create(Player((float)value[0], (float)value[1]));
						break;
					default:
						break;
					}
				}
				now = (now + 1) % 3;
			}
			else if (_msg[i] >= '0' && _msg[i] <= '9')
				temp = temp * 10 + _msg[i] - '0';
		}
	}
}