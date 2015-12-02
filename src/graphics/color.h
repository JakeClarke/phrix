#ifndef _H_PHRIX_GRAPHICS_COLOR
#define _H_PHRIX_GRAPHICS_COLOR

namespace phrix {
	namespace graphics {
		struct Color {
			float R, G, B, A;

			static Color Black() {
				return{ 0,0,0,1 };
			}

			static Color White() {
				return{ 1,1,1,1 };
			}

			static Color Blue() {
				return{ 0,0,1,1 };
			}

			static Color Red() {
				return{ 1,0,0,1 };
			}

			static Color Green() {
				return{ 0,1,0,1 };
			}
		};
	}
}

#endif