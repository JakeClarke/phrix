#include "scene.h"
#include <algorithm>

using namespace phrix::game;

Scene::Scene(Game *game) :parent(game) {
}
void Scene::add(std::unique_ptr<Entity> &e) {
	if (this->m_mainCamera == nullptr && e->hasComponent<Camera>()) {
		this->m_mainCamera = e->getComponent<Camera>();
	}
	this->m_entities.push_back(std::move(e));
}

void Scene::destory(Entity *e) {}

void Scene::update()
{
	std::for_each(m_entities.begin(), m_entities.end(), [](std::unique_ptr<Entity> &e) {
		// if this entity has not been started now is the time
		if (!e->m_started) {
			std::for_each(
				e->m_components.begin(),
				e->m_components.end(), 
				[](auto& comp) {
				comp.second.get()->onStart();
			});
			e->m_started = true;
		}

		// update the components
		std::for_each(
			e->m_components.begin(),
			e->m_components.end(),
			[](auto& comp) {
			comp.second.get()->onUpdate();
		});
	});
}

Scene::~Scene() {}
