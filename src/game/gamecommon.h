#ifndef _H_GAME_GAMECOMMON
#define _H_GAME_GAMECOMMON

#include <chrono>

struct GameTime {
	std::chrono::duration<double, std::milli> totalGameTime;
	std::chrono::duration<double, std::milli> elapsedGameTime;
};

#endif