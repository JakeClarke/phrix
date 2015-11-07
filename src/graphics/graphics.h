#ifndef _H_GRAPHICS_GRAPHICS
#define _H_GRAPHICS_GRAPHICS

#include "renderer\renderer.h"
#include "servicemanager.h"

namespace phrix {
	namespace graphics {

		struct Color {
			float Red, Greeen, Blue, Alpha;
		};

		class GraphicsService : public phrix::game::Service {

		};
	}
}


#endif
