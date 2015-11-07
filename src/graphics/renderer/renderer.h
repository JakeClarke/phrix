#ifndef _H_GRAPHICS_RENDERER_RENDERER
#define _H_GRAPHICS_RENDERER_RENDERER

#include "../graphics.h"
#include <memory>

namespace phrix {
	namespace graphics {
		struct Color;

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
				virtual void clear(Color) = 0;
				virtual std::unique_ptr<RendererContext> getContext(RendererContextType) = 0;
			};

			Renderer* getRenderer();
		}
	}
}

#endif