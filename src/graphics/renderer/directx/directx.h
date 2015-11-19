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
					virtual bool init() override;
					virtual std::unique_ptr<RendererContext> getContext(RendererContextType) override;
					virtual void present() override;
					virtual void clear(Color c) override;
				private:
					void loadContent();
				};

				Renderer* getRenderer();
			}
		}
	}
}

#endif