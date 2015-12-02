#ifndef _H_SRC_GAME_ENTITY
#define _H_SRC_GAME_ENTITY

#include "transforms.h"
#include <memory>
#include <vector>

namespace phrix {
	namespace game {
		class Entity;

		class EntityComponent {
			friend class Entity;
		public:
			Entity * getParent() {
				return m_parent;
			}

			bool isEnabled() {
				return m_enabled;
			}

			void setEnabled(bool enabled) {
				m_enabled = enabled;
			}

		protected:
			virtual void onStart() {}
			virtual void onRender() {}
			virtual void onUpdate() {}
			virtual void onDestroy() {}
		private:
			Entity * m_parent;
			
			bool m_enabled;
		};

		class Scene;

		class Entity
		{
		public:
			Transforms transforms;

			void AddComponent(std::unique_ptr<EntityComponent> &c) {
				c->m_parent = this;
				this->components.push_back(std::move(c));
			}

			~Entity() {};
		private:
			std::vector<std::unique_ptr<EntityComponent>> components;
			Scene * m_scene;
		};
	}
}

#endif