#include "servicemanager.h"
#include "game.h"

#include <algorithm>

using namespace phrix::game;

ServiceManager::ServiceManager(Game* game) : parent(game), started(false) {}

void ServiceManager::start() {
 		started = true;

		std::for_each(services.begin(), services.end(), [](auto & serv) {
			serv.second->start();
		});
}

void phrix::game::ServiceManager::tick()
{
	std::for_each(services.begin(), services.end(), [](auto & serv) {
		serv.second->tick();
	});
}
