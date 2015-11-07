#include "renderer.h"
#include "directx\directx.h"

using namespace phrix::graphics::renderer;

Renderer* phrix::graphics::renderer::getRenderer()
{
	return directx::getRenderer();
}
