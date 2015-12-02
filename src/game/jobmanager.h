#ifndef _H_GAME_JOB_MANAGER
#define _H_GAME_JOB_MANAGER

#include <atomic>
#include <thread>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>

namespace phrix {
namespace game {

class JobManager;

class Job {
  friend class JobManager;
public:
	virtual ~Job() {}
	void addDependancy(Job *);
	JobManager * getParent() {
		return parent;
	}
 protected:
  Job() : locked(true) {}
  Job(bool locked) : locked(locked) {}

  virtual void run() = 0;
  
 private:
 	JobManager * parent;
 	void signalDependant();
 	void awaitDependancies();
  bool locked;
  std::vector<Job*> dependancies;
  std::atomic<int> remainingDependancies;

  std::condition_variable dependanciesCV;
};

class JobManager {
	friend class Job;
 public:
  JobManager(int max, int min);
  JobManager(int max);
  void sched(std::unique_ptr<Job> &);
  void wait();
  void start();
  ~JobManager();

 private:
  std::unique_ptr<Job> popJob();
  void sleepIfUnneeded();
  void notifyBlocked();
  void notifyUnblocked();
  std::thread createThread();

  std::mutex jobQueueMutex;
  std::queue<std::unique_ptr<Job>> jobQueue;
  std::queue<std::unique_ptr<Job>> lockedJobQueue;
  /// cv to notify threads waiting on new jobs.
  std::condition_variable jobCv;
  /// mutex for threads that need to sleep to wait on.
  std::mutex workerSleep;
  /// cv to wait and notify sleeping threads.
  std::condition_variable workerThreadSleepCv;

  /// max number of threads that this job manager suppots.
  int maxThreads;
  /// min number of threads that this job manager runs at start.
  /// hint: set to hw threads -1 .
  int minThreads;
  /// suggested number of threads that should be running at any time.
  std::atomic<int> suggestedNumThreads;
  /// current number of threads that are seeking work.
  std::atomic<int> activeThreads;

  std::unique_ptr<std::thread[]> workerthreads;
  std::atomic<int> pending;
  bool started;
  bool exiting;
  std::atomic<bool> processing;

  int queuedJobs;
};
}
}

#endif