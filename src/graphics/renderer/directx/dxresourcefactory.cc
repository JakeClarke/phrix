#include "dxresourcefactory.h"

using namespace phrix::graphics;
using namespace phrix::graphics::renderer::directx;

std::unique_ptr<Texture2D> DxGraphicsResourceFactory::makeTexture2D(int width, int height)
{
	return std::unique_ptr<Texture2D>();
}

std::unique_ptr<RenderTexture2D> DxGraphicsResourceFactory::makeRenderTexture2D(int width, int height)
{
	return std::unique_ptr<RenderTexture2D>();
}

std::unique_ptr<VertexShader> DxGraphicsResourceFactory::makeVertexShader()
{
	return std::unique_ptr<VertexShader>();
}

std::unique_ptr<PixelShader> DxGraphicsResourceFactory::makePixelShader()
{
	return std::unique_ptr<PixelShader>();
}
