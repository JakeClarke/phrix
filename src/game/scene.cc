#include "scene.h"

using namespace phrix::game;

Scene::Scene(Game *game):parent(game) {}

void Scene::Add(std::unique_ptr<Entity> &e) {}
void Scene::Destory(Entity *e) {}
Scene::~Scene() {}
