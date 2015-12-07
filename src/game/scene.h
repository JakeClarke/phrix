#ifndef _H_SRC_GAME_SCENE
#define _H_SRC_GAME_SCENE

#include "entity.h"
#include "camera.h"

#include <memory>
#include <vector>

namespace phrix {
namespace game {
class Game;

class Scene {
 public:
  Scene(Game *game);
  void add(std::unique_ptr<Entity> &e);
  void destory(Entity *e);

  void setMainCamera(Camera * c) {
	  m_mainCamera = c;
  }

  Camera * getMainCamera() {
	  return m_mainCamera;
  }

  std::vector<Camera*> getCameras();

  void update();

  ~Scene();
 private:
 	Game *parent;
	Camera *m_mainCamera;
	std::vector<std::unique_ptr<Entity>> m_entities;
};
}
}

#endif