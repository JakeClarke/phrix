#ifndef _H_GAME_GAME
#define _H_GAME_GAME

#include "gamecommon.h"
#include "servicemanager.h"
#include "jobmanager.h"
#include "scene.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <map>

namespace phrix {
namespace game {
	class JobManager;
	class ServiceManager;

class Game {
 public:
 	Game();
 	~Game();
 	
  void run();
  void exit();

 private:
  std::chrono::high_resolution_clock::time_point startTime;

  std::unique_ptr<JobManager> jobManager;
  std::unique_ptr<ServiceManager> services;

  std::unique_ptr<Scene> scene;

  bool running;
  bool exiting;
};
}
}

#endif