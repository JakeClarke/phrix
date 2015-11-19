#ifndef _H_SRC_GAME_SERVICE
#define _H_SRC_GAME_SERVICE

#include <memory>
#include <map>
#include <typeindex>

namespace phrix {
namespace game {

class ServiceManager;

class Service {
  friend class ServiceManager;

 public:
	 virtual ~Service() {};

 protected:
  virtual void start() {}
  virtual void tick() {}
  ServiceManager *getParent() { return parent; }

 private:
  void setParent(ServiceManager *s) { parent = s; }
  ServiceManager *parent;
};

class Game;

class ServiceManager {
 public:
  ServiceManager(Game *p);

  void start();

  template <class t>
  t get() {
    std::unique_ptr<Service> &serv = services[typeid(t)];
    return static_cast<t *>(serv.get());
  }

  template <class t>
  t create() {
    std::unique_ptr<Service> &serv = services[typeid(t)];
    if (serv) {
      return static_cast<t *>(serv.get());
    }
    serv = std::unique_ptr<Service>(new t());
    serv->setParent(this);
    if (started) {
      serv->start();
    }
    return static_cast<t *>(serv.get());
  }

 private:
  std::map<std::type_index, std::unique_ptr<Service>> services;
  Game *parent;
  bool started;
};
}
}

#endif