#include "game.h"

#include <iostream>
#include <SDL.h>

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

  SDL_Window *window;

  {
    window = SDL_CreateWindow("phrix",                  // window title
                              SDL_WINDOWPOS_UNDEFINED,  // initial x position
                              SDL_WINDOWPOS_UNDEFINED,  // initial y position
                              800,                      // width, in pixels
                              600,                      // height, in pixels
                              0);
  }

  if (window == nullptr) {
    std::cerr << "Could not create window: %s\n" << SDL_GetError() << std::endl;
    return;
  }

  SDL_Renderer *render =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    std::cerr
        << "Failed to create hardware renderer: SDL_CreateRenderer Error: "
        << SDL_GetError() << std::endl;
    std::cerr << "Falling back to software rendering..." << std::endl;

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (render == nullptr) {
      std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
      std::cerr << "Failed to create any renderer." << std::endl;
      return;
    }
  }

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
    j = std::unique_ptr<Job>(new longJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new longJob());
    jobManager.sched(j);
    j = std::unique_ptr<Job>(new testJob());
    jobManager.sched(j);
    jobManager.wait();
    SDL_RenderClear(render);
    // do the rendering.
    SDL_RenderPresent(render);
    std::cout << "frame end" << std::endl;
  }

  std::cout << "GameRun exit" << std::endl;
  running = false;
}

void Game::exit() { exiting = true; }