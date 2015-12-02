#include "game.h"

#include <iostream>
#include "SDL.h"
#include "../graphics/graphics.h"

using namespace phrix::game;

constexpr std::chrono::duration<double, std::milli> targetFrameDuration =
    (std::chrono::milliseconds(1000)) / 60.0;

Game::Game() : services(new ServiceManager(this)), running(false), exiting(false), jobManager(new JobManager(4)), scene(new Scene(this)) {
	services->create<graphics::GraphicsService>();
}

Game::~Game() { exit(); }

void Game::run() {
  if (exiting || running) {
    return;
  }

  startTime = std::chrono::high_resolution_clock::now();
  exiting = false;
  running = true;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    return;
  }

  SDL_version compiled;
  SDL_version linked;

  SDL_VERSION(&compiled);
  std::cout << "SDL compiled version" << int(compiled.major) << "." << int(compiled.minor) << "." << int(compiled.patch) << std::endl;
  SDL_GetVersion(&linked);
  std::cout << "SDL linked version" << int(linked.major) << "." << int(linked.minor) << "." << int(linked.patch) << std::endl;

  jobManager->start();
  services->start();

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
	if (!scene)
	{
		std::cerr << "No scene!" << std::endl;
	}
	else {
		scene->update();
	}
	jobManager->wait();
    // do the rendering.
    std::cout << "frame end" << std::endl;
  }

  std::cout << "GameRun exit" << std::endl;
  running = false;
}

void Game::exit() { exiting = true; }