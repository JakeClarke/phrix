#ifndef _H_SRC_GAME_SCENE
#define _H_SRC_GAME_SCENE

#include "entity.h"

#include <memory>

namespace phrix {
namespace game {
class Game;

class Scene {
 public:
  Scene(Game *game);
  void Add(std::unique_ptr<Entity> &e);
  void Destory(Entity *e);
  ~Scene();
 private:
 	Game *parent;

};
}
}

#endif