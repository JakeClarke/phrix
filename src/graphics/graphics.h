#ifndef _H_GRAPHICS_GRAPHICS
#define _H_GRAPHICS_GRAPHICS

#include "renderer\renderer.h"
#include "servicemanager.h"
#include "graphicsscene.h"

namespace phrix {
	namespace graphics {

		class GraphicsService : public phrix::game::Service {
			void tick() override;
			void start() override;
			std::unique_ptr<renderer::Renderer> renderer;
		};
	}
}


#endif
