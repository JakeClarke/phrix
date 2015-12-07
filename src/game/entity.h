#ifndef _H_SRC_GAME_ENTITY
#define _H_SRC_GAME_ENTITY

#include "transforms.h"
#include <memory>
#include <map>
#include <typeindex>

namespace phrix {
	namespace game {
		class Entity;
		class Scene;

		class EntityComponent {
			friend class Entity;
			friend class Scene;
		public:
			Entity * getEntity() {
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

		class Entity
		{
			friend class Scene;
		public:
			Transforms transforms;

			template<class t>
			t* addComponent() {
				std::unique_ptr<EntityComponent> &comp = m_components[typeid(t)];
				if (comp) {
					return static_cast<t *>(comp.get());
				}
				comp = std::unique_ptr<EntityComponent>(new t());
				comp->m_parent = this;
				if (m_started) {
					comp->onStart();
				}
				return static_cast<t *>(comp.get());
			}

			template<class t>
			t* getComponent() {
				std::unique_ptr<EntityComponent> &comp = m_components[typeid(t)];
				return static_cast<t *>(comp.get());
			}

			template<class t>
			bool hasComponent() {
				return getComponent<t>() != nullptr;
			}

			~Entity() {};
		private:
			std::map<std::type_index, std::unique_ptr<EntityComponent>> m_components;
			Scene * m_scene;
			bool m_started;
		};
	}
}

#endif