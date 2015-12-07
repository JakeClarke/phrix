#include "graphics.h"

void phrix::graphics::GraphicsService::tick()
{
	auto renderer = phrix::graphics::renderer::getRenderer();
	renderer->renderGame(this->getParent()->getParent()->getJobManager()->getGame());
}

void phrix::graphics::GraphicsService::start()
{
	auto renderer = phrix::graphics::renderer::getRenderer();
	renderer->init();
}
