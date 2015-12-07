#ifndef _H_PHRIX_CAMERA
#define _H_PHRIX_CAMERA

#include "entity.h"
#include "../graphics/color.h"
#include "../graphics/rendertexture2d.h"

namespace phrix {
	namespace game {
		enum CameraClear
		{
			CameraClearAll, CameraClearDepth, CameraClearNone
		};

		class Camera : public EntityComponent {
		public:
			Mat4F view;
			graphics::Color clearColor;
			CameraClear clearFlag;
			graphics::RenderTexture2D *target;
		};
	}
}

#endif