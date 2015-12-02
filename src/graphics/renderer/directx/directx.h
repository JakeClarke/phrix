#ifndef _H_PHRIX_GAME_GRAPHICS_RENDERERS_DIRECTX_DIRECTX
#define _H_PHRIX_GAME_GRAPHICS_RENDERERS_DIRECTX_DIRECTX

#include "../renderer.h"

namespace phrix {
	namespace graphics {
		namespace renderer {
			namespace directx {
				class DirectxRenderer : public Renderer {
				public:
					// Inherited via Renderer
					bool init() override;
					void present() override;
					void renderScene(Scene * s) override;
					std::unique_ptr<GraphicsResourceFactory> getResourceFactory() override;
				private:
					void loadContent();
				};

				Renderer* getRenderer();
			}
		}
	}
}

#endif