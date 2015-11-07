#include "jobmanager.h"

#include <iostream>
#include <cassert>

using namespace phrix::game;

int maxJobs = 1024;

JobManager::JobManager(int maxThreads, int minThreads)
    : maxThreads(maxThreads),
      minThreads(minThreads),
      suggestedNumThreads(minThreads),
      started(false),
      exiting(false),
      pending(0),
      processing(false),
		queuedJobs(0) {}

JobManager::JobManager(int maxThreads)
    : maxThreads(maxThreads),
      minThreads(std::thread::hardware_concurrency()),
      suggestedNumThreads(minThreads),
      started(false),
      exiting(false),
      pending(0),
      processing(false),
	  queuedJobs(0){}

void JobManager::sched(std::unique_ptr<Job>& job) {
  assert(job);
  std::unique_lock<std::mutex> jl(jobQueueMutex);
  if (queuedJobs < maxJobs)
  {
	  queuedJobs++;
	  if (job->locked) {
		  lockedJobQueue.emplace(std::move(job));
	  }
	  else {
		  jobQueue.emplace(std::move(job));
	  }
  }
  else {
	  std::cerr << "too many jobs. dropping job." << std::endl;
  }

}

void JobManager::start() {
  if (!started) {
    started = true;
    activeThreads = 0;
    suggestedNumThreads = minThreads;
    workerthreads = std::unique_ptr<std::thread[]>(new std::thread[maxThreads]);
    for (int i = 0; i < maxThreads; ++i) {
      workerthreads[i] = createThread();
    }
  }
}

void JobManager::sleepIfUnneeded() {
  while (true) {
    int active = activeThreads;
    if (active > suggestedNumThreads) {
      std::cout << active << "/" << suggestedNumThreads << std::endl;
      if (activeThreads.compare_exchange_strong(active, active - 1)) {
      	--pending;
        std::unique_lock<std::mutex> l(workerSleep);
        workerThreadSleepCv.wait(
            l, [this]() { return activeThreads < suggestedNumThreads || exiting; });
        ++activeThreads;
        ++pending;
      }
    } else {
      return;
    }
  }
}

std::thread JobManager::createThread() {
  ++activeThreads;
  ++pending;
  return std::thread([this]() {
    while (!exiting) {
      sleepIfUnneeded();
      auto job = popJob();
      assert(job || exiting);
      if (job) {
        job->awaitDependancies();
        if (job->locked) {
          job->run();
        } else {
          // we don't want to block the main thread for long jobs.
          --pending;
          job->run();
          ++pending;
        }
        job->signalDependant();
      }
    }
    --pending;
    --activeThreads;
  });
}

void JobManager::wait() {
  processing = true;
  jobCv.notify_all();
  while (true) {
    std::unique_ptr<Job> job;
    {
      // main thread should only consume work that we can expect to complete in
      // the current frame.
      std::unique_lock<std::mutex> jl(jobQueueMutex);
      assert(jl);
      if (!lockedJobQueue.empty()) {
        job = std::move(lockedJobQueue.front());
        lockedJobQueue.pop();
        assert(job);
      } else {
        break;
      }
    }

    job->run();
  }

  processing = false;

  while (pending > 0) {
    std::this_thread::yield();
  }
}

std::unique_ptr<Job> JobManager::popJob() {
  std::unique_ptr<Job> res;
  while (true) {
    std::unique_lock<std::mutex> jl(jobQueueMutex);
    assert(jl);
    // this is kind of messy buy i'd rather maintain the lock then reaquire.
    if (!processing && !exiting) {
      --pending;
      jobCv.wait(jl, [this]() { return processing || exiting; });
      ++pending;
    }

    // if we woke up due to a exit signal don't process anything.
    if (exiting) {
      return res;
    }

    // todo a fairer selection from the queues.
    if (!lockedJobQueue.empty()) {  // frame locked jobs second.
      res = std::move(lockedJobQueue.front());
      lockedJobQueue.pop();
	  --queuedJobs;
      return res;
    } else if (!jobQueue.empty()) {  // long jobs next.
      res = std::move(jobQueue.front());
      jobQueue.pop();
	  --queuedJobs;
      return res;
    } else {
      std::this_thread::yield();
    }
  }
  return res;
}

void JobManager::notifyBlocked() {
  if (suggestedNumThreads < maxThreads) {
    ++suggestedNumThreads;
    workerThreadSleepCv.notify_one();
  }
}

void JobManager::notifyUnblocked() {
  if (suggestedNumThreads > minThreads) {
    --suggestedNumThreads;
  }
}

JobManager::~JobManager() {
  if (started) {
    exiting = true;
    jobCv.notify_all();
    workerThreadSleepCv.notify_all();
    for (int i = 0; i < maxThreads; ++i) {
      workerthreads[i].join();
    }
  }
}

void Job::addDependancy(Job* j) {
  this->remainingDependancies++;
  j->dependancies.emplace_back(j);
}

void Job::signalDependant() {
  for (auto j : dependancies) {
    j->remainingDependancies--;
    if (j->remainingDependancies == 0) {
      j->dependanciesCV.notify_one();
    }
  }
}

void Job::awaitDependancies() {
  if (remainingDependancies > 0) {
    std::mutex dependanciesBlock;
    std::unique_lock<std::mutex> l(dependanciesBlock);
    parent->notifyBlocked();
    dependanciesCV.wait(l, [this]() { return remainingDependancies == 0; });
    parent->notifyUnblocked();
  }
}