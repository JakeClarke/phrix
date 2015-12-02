#ifndef _H_SRC_GAME_SERVICE
#define _H_SRC_GAME_SERVICE

#include "game.h"
#include "service.h"
#include <memory>
#include <map>
#include <typeindex>

namespace phrix {
namespace game {

class Game;

class ServiceManager {
 public:
  ServiceManager(Game *p);

  void start();

  template <class t>
  t* get() {
    std::unique_ptr<Service> &serv = services[typeid(t)];
    return static_cast<t *>(serv.get());
  }

  template <class t>
  t* create() {
    std::unique_ptr<Service> &serv = services[typeid(t)];
    if (serv) {
      return static_cast<t *>(serv.get());
    }
    serv = std::unique_ptr<Service>(new t());
    serv->parent = this;
    if (started) {
      serv->start();
    }
    return static_cast<t *>(serv.get());
  }

  Game* getParent() {
	  return parent;
  }

 private:
  std::map<std::type_index, std::unique_ptr<Service>> services;
  Game *parent;
  bool started;
};
}
}

#endif