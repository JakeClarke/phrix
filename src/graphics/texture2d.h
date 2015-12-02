#ifndef _H_PHRIX_GRAPHICS_TEXTURE2D
#define _H_PHRIX_GRAPHICS_TEXTURE2D

#include "texture.h"

namespace phrix {
	namespace graphics {
		class Texture2D : public Texture {
		public:
			Texture2D(int width, int height) : m_width(width), m_height(height) {}
			int getWidth() { return m_width; }
			int getHeight() { return m_height; }
			virtual void setData(Color * c, size_t count) = 0;
			virtual void copyTo(Color * c, size_t num) = 0;
			virtual ~Texture2D() = 0;
		private:
			int m_width;
			int m_height;
		};
	}
}

#endif