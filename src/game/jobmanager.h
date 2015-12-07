#ifndef _H_GAME_JOB_MANAGER
#define _H_GAME_JOB_MANAGER

#include <atomic>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace phrix {
	namespace game {
		class Game;
		class JobManager;

		class Job {
			friend class JobManager;
		public:
			Job() : remainingDependancies(), locked(true) {
				remainingDependancies = 0;
			}
			Job(bool locked) :remainingDependancies(0), locked(locked) {}
			virtual ~Job() {}
			void addDependancy(Job *);
			JobManager * getParent() {
				return parent;
			}
		protected:

			virtual void run() = 0;

		private:
			std::atomic<int> remainingDependancies;
			JobManager * parent;
			void signalDependant();
			void awaitDependancies();
			bool locked;
			std::vector<Job*> dependancies;


			std::condition_variable dependanciesCV;
		};

		class JobManager {
			friend class Job;
		public:
			JobManager(Game *game);
			void sched(std::unique_ptr<Job> &);
			void wait();
			void start();
			size_t numJobs();
			Game *getGame() { return m_game; }
			~JobManager();

		private:
			std::unique_ptr<Job> popJob();
			void notifyBlocked();
			void notifyUnblocked();
			std::thread createThread();

			static constexpr size_t maxJobs = 256;

			Game* m_game;
			std::unique_ptr<Job> m_jobs[maxJobs];
			size_t m_head;
			size_t m_tail;

			std::mutex jobQueueMutex;
			/// cv to notify threads waiting on new jobs.
			std::condition_variable jobCv;
			/// main thread cv
			std::condition_variable mainThreadSleepCv;


			/// min number of threads that this job manager runs at start.
			/// hint: set to hw threads -1 .
			int minThreads;

			/// max number of threads that this job manager suppots.
			int maxThreads;

			std::unique_ptr<std::thread[]> workerthreads;
			std::atomic<int> m_active;
			bool started;
			bool exiting;
			std::atomic<bool> processing;
		};
	}
}

#endif