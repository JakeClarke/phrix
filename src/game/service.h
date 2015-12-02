#ifndef _H_GAME_SERVICE
#define _H_GAME_SERVICE

#include "servicemanager.h"

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
			virtual void term() {}
			ServiceManager *getParent() { return parent; }

		private:
			ServiceManager *parent;
		};
	}
}




#endif