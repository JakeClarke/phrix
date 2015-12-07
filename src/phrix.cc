#include "game/game.h"

#include "game/entity.h"
#include "game/camera.h"

#include <string>
#include <iostream>

int main(int argc, char const *argv[]) {
	phrix::game::Game game;
	std::unique_ptr<phrix::game::Entity> e(new phrix::game::Entity());
	auto mainCam = e->addComponent<phrix::game::Camera>();
	mainCam->clearColor = phrix::graphics::Color::Red();

	game.getScene()->add(e);
	game.run();
  return 0;
}