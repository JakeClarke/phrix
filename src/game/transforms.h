#ifndef _H_PHRIX_TRANSFORMS
#define _H_PHRIX_TRANSFORMS

#include "../mat.h"

namespace phrix {
	namespace game {
		class Transforms {
		public:
			Vector3F getPosition() {
				return m_position;
			}

			void setPosition(Vector3F pos) {
				m_position = pos;
			}

			Transforms * getParent() {
				return m_parent;
			}

			void setParent(Transforms * parent) {
				m_parent = parent;
			}

			operator Mat4F() const;

		private:
			Transforms *m_parent;
			Vector3F m_position;
		};
	}
}

#endif