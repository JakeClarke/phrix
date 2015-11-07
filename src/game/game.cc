#include "game.h"

#include <iostream>
#include "SDL.h"
#include "../graphics/graphics.h"

using namespace phrix::game;

constexpr std::chrono::duration<double, std::milli> targetFrameDuration =
    (std::chrono::milliseconds(1000)) / 60.0;

Game::Game() : services(this), running(false), exiting(false), jobManager(4) {
}

Game::~Game() { exit(); }

class testJob : public Job {
 public:
  testJob() : Job(true) {}

 protected:
  void run() override {
    static int r = 0;
    r++;
    std::cout << "test Job ran! " << r << std::endl;
  }
};

class longJob : public Job {
 public:
  longJob() : Job(false) {}

 protected:
  void run() override {
    static int r = 0;
    r++;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "long job ran! " << r << std::endl;
  }
};

void Game::run() {
  if (exiting || running) {
    return;
  }

  startTime = std::chrono::high_resolution_clock::now();
  exiting = false;
  running = true;

  jobManager.start();
  services.start();

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  std::cout << "SDL compiled version" << compiled.major << "." << compiled.minor << "." << compiled.patch << std::endl;
  SDL_GetVersion(&linked);
  std::cout << "SDL linked version" << linked.major << "." << linked.minor << "." << linked.patch << std::endl;


  auto  renderer = phrix::graphics::renderer::getRenderer();
  renderer->init();

  while (!exiting) {
    std::cout << "frame start" << std::endl;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          exiting = true;
          break;
      }
    }
    auto j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new longJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    jobManager.wait();


	graphics::Color c = {1.0f,0,0,0 };
	renderer->clear(c);
    // do the rendering.
    std::cout << "frame end" << std::endl;
	renderer->present();
  }

  std::cout << "GameRun exit" << std::endl;
  running = false;
}

void Game::exit() { exiting = true; }