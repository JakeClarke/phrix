#include "assetmanager.h"

#include <iostream>

using namespace phrix::assetmanager;

bool AssetManager::update(){
	auto a = popWaiting();
	if (a) {
		std::cout << "(AssetManager) Loading asset: " << a->name << std::endl;
		a->load();
	} else {
		return false;
	}
	return hasWaiting();
}

std::shared_ptr<LoadableAsset> AssetManager::popWaiting() {
	std::lock_guard<std::mutex> lock(loadListMutex);
	auto a = std::move(asyncLoaded.front());
	asyncLoaded.pop();
	return a;
}

bool AssetManager::hasWaiting() {
	std::lock_guard<std::mutex> lock(loadListMutex);
	return asyncLoaded.size() > 0;
}

