#ifndef _H_PHRIX_ASSETMANAGER_ASSETMANGER
#define _H_PHRIX_ASSETMANAGER_ASSETMANGER

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <future>
#include <mutex>

namespace phrix {
namespace assetmanager {

class AssetManager;

class LoadableAsset {
  friend class AssetManager;

 public:
  LoadableAsset() : loaded(false) {}
  bool isLoaded() { return loaded; }

 protected:
  virtual void loadAsync() = 0;
  virtual void load() = 0;

  virtual void unloadAsync() = 0;
  virtual void unload() = 0;

 private:
  AssetManager* parent;
  std::string name;
  bool loaded;
};

class AssetManager {
 private:
 public:
  template <class t>
  std::shared_ptr<t> load(const char* asset) {
    std::shared_ptr<t> res;
    std::string name = asset;
    std::weak_ptr<LoadableAsset>& stored = map[name];
    if ((res = std::dynamic_pointer_cast<t>(stored.lock()))) {
      return res;
    }
    // we need a new reference.
    res = std::make_shared<t>();
    stored = res;
    res->parent = this;
    res->name = name;
    std::async(std::launch::async, [&, res] {
      static_cast<LoadableAsset*>(res.get())->loadAsync();
      std::lock_guard<std::mutex> lock(loadListMutex);
      asyncLoaded.push(res);
    });
    return res;
  }

  bool update();
  bool hasWaiting();
  void unload();

 private:
  std::shared_ptr<LoadableAsset> popWaiting();

  std::map<std::string, std::weak_ptr<LoadableAsset>> map;
  std::queue<std::shared_ptr<LoadableAsset>> asyncLoaded;
  std::mutex loadListMutex;
};
}
}

#endif
