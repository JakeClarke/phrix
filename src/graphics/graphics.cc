#include "graphics.h"

void phrix::graphics::GraphicsService::tick()
{

}

void phrix::graphics::GraphicsService::start()
{
	auto renderer = phrix::graphics::renderer::getRenderer();
	renderer->init();
}
