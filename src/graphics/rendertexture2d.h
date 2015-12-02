#ifndef _H_PHRIX_GRAPHICS_RENDERTEXTURE2D
#define _H_PHRIX_GRAPHICS_RENDERTEXTURE2D

#include "texture2d.h"

namespace phrix {
	namespace graphics {
		class RenderTexture2D : public Texture2D {
			RenderTexture2D(int width, int height) : Texture2D(width, height) {}
		};
	}
}

#endif // !_H_PHRIX_GRAPHICS_RENDERTEXTURE2D
