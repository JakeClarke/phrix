#include "servicemanager.h"
#include "game.h"

using namespace phrix::game;

ServiceManager::ServiceManager(Game* game) : parent(game) {}

void ServiceManager::start() {
 		started = true;
 		for(auto & serv : services) {
 			serv.second->start();
 		}
}