#pragma once
#include <list>
#include <Windows.h>
#include <algorithm>

//Easily identify important keys
enum KEYS { KEY_W = 0x57, KEY_A = 0x41, KEY_S = 0x53, KEY_D = 0x44, KEY_SPACE = 0x20, KEY_ESC = 0x1B };
//Easily Identify mouse input
enum MOUSE { M_LEFT = 0x01, M_RIGHT, M_MID = 0x04 };

class EventHandler
{
public:

	EventHandler()
	{
	}

	~EventHandler()
	{
	}

	void ReadInput()
	{
		for (char c : Keys)
		{
			if (GetKeyState(c) & 0x8000) Events.push_back(c);
			else
			{
				if (ContainsKey(0, c))
				{
					Events.remove(c);
					RemovedKeys.push_back(c);
				}
			}
		}
	}

	enum LISTS { EVENTS = 0, REMOVED };
	bool ContainsKey(char list, char Key)
	{
		if (list == EVENTS)
		{
			auto iter = std::find(Events.begin(), Events.end(), Key);
			if (iter != Events.end()) return true;
			return false;
		}

		if (list == REMOVED)
		{
			auto iter = std::find(RemovedKeys.begin(), RemovedKeys.end(), Key);
			if (iter != RemovedKeys.end()) return true;
			return false;
		}
		
	}

	std::list<char> Events;
	std::list<char> RemovedKeys;
	char Keys[9] = { KEY_W, KEY_A, KEY_S, KEY_D, KEY_SPACE, KEY_ESC, M_LEFT, M_RIGHT, M_MID};

private:
};

