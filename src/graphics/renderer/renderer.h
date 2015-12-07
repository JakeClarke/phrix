#ifndef _H_GRAPHICS_RENDERER_RENDERER
#define _H_GRAPHICS_RENDERER_RENDERER

#include "../graphicsresourcefactory.h"
#include "../color.h"
#include "../game/scene.h"
#include <memory>

namespace phrix {
	namespace graphics {
		namespace renderer {

			enum RendererContextType {
				RendererContextImmediate,
				RendererContextDeffered,
			};

			class RendererContext {
			public:
				virtual void clear() = 0;
				virtual void begin() = 0;
				virtual void end() = 0;
			};

			class Renderer {
			public:
				virtual bool init() = 0;
				virtual void present() = 0;
				virtual void renderGame(phrix::game::Game *g) = 0;
				virtual std::unique_ptr<GraphicsResourceFactory> getResourceFactory() = 0;
			};

			Renderer* getRenderer();
		}
	}
}

#endif