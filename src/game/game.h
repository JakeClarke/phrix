#ifndef _H_GAME_GAME
#define _H_GAME_GAME

#include "gamecommon.h"
#include "servicemanager.h"
#include "../assetmanager/assetmanager.h"
#include "jobmanager.h"
#include "scene.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <map>

namespace phrix {
namespace game {

class Game {
 public:
 	Game();
 	~Game();
 	
  void run();
  void exit();
  ServiceManager *Services() {
  	return &services;
  }

 private:
  std::chrono::high_resolution_clock::time_point startTime;

  JobManager jobManager;
  ServiceManager services;

  std::unique_ptr<Scene> scene;

  bool running;
  bool exiting;
};
}
}

#endif