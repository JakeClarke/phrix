#ifndef _H_PHRIX_CAMERA
#define _H_PHRIX_CAMERA

#include "entity.h"

namespace phrix {
	namespace game {
		class Camera : public EntityComponent {
		public:
			Mat4F view;
		};
	}
}

#endif