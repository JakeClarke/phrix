#ifndef _H_PHRIX_GRAPHICS_RENDERER_DIRECTX_RESOURCEFACTORY
#define _H_PHRIX_GRAPHICS_RENDERER_DIRECTX_RESOURCEFACTORY

#include "../../graphicsresourcefactory.h"

namespace phrix {
	namespace graphics {
		namespace renderer {
			namespace directx {
				class DxGraphicsResourceFactory : public GraphicsResourceFactory {
					// Inherited via GraphicsResourceFactory
					virtual std::unique_ptr<Texture2D> makeTexture2D(int width, int height) override;
					virtual std::unique_ptr<RenderTexture2D> makeRenderTexture2D(int width, int height) override;
					virtual std::unique_ptr<VertexShader> makeVertexShader() override;
					virtual std::unique_ptr<PixelShader> makePixelShader() override;
				};
			}
		}
	}
}

#endif // !_H_PHRIX_GRAPHICS_RENDERER_DIRECTX_RESOURCEFACTORY
