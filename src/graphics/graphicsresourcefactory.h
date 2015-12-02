#ifndef _H_PHRIX_GRAPHICS_GRAPHICSRESOURCEFACTORY
#define _H_PHRIX_GRAPHICS_GRAPHICSRESOURCEFACTORY

#include "texture2d.h"
#include "rendertexture2d.h"
#include "pixelshader.h"
#include "vertexshader.h"
#include <memory>

namespace phrix {
	namespace graphics {
		class GraphicsResourceFactory {
			virtual std::unique_ptr<Texture2D> makeTexture2D(int width, int height) = 0;
			virtual std::unique_ptr<RenderTexture2D> makeRenderTexture2D(int width, int height) = 0;
			virtual std::unique_ptr<VertexShader> makeVertexShader() = 0;
			virtual std::unique_ptr<PixelShader> makePixelShader() = 0;
		};
	}
}

#endif // !_H_PHRIX_GRAPHICS_GRAPHICSRESOURCEFACTORY
