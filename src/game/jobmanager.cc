#include "jobmanager.h"
#include "jobmanager.h"

#include <iostream>
#include <cassert>

using namespace phrix::game;

JobManager::JobManager(Game* game)
	: m_game(game),
	m_head(0),
	m_tail(0),
	minThreads(std::thread::hardware_concurrency()),
	maxThreads(2 * minThreads),
	started(false),
	exiting(false),
	m_active(0),
	processing(false) {}

void JobManager::sched(std::unique_ptr<Job>& job) {
	assert(job);
	job->parent = this;
	std::unique_lock<std::mutex> jl(jobQueueMutex);
	if (numJobs() < maxJobs)
	{
		m_jobs[m_head] = std::move(job);
		m_head = (m_head + 1) % maxJobs;
	}
	else {
		std::cerr << "too many jobs. dropping job." << std::endl;
	}

}

void JobManager::start() {
	if (!started) {
		started = true;
		std::unique_lock<std::mutex> jl(jobQueueMutex);
		workerthreads = std::unique_ptr<std::thread[]>(new std::thread[maxThreads]);
		for (int i = 0; i < maxThreads; ++i) {
			workerthreads[i] = createThread();
		}
		mainThreadSleepCv.wait(jl, [this]() {
			return m_active == 0;
		});
	}
}

size_t phrix::game::JobManager::numJobs()
{
	return m_head >= m_tail ? m_head - m_tail : (m_head + maxJobs) - m_tail;
}

std::thread JobManager::createThread() {
	++m_active;
	return std::thread([this]() {
		while (!exiting) {
			auto job = popJob();
			assert(job || exiting);
			if (job) {
				job->awaitDependancies();
				if (job->locked) {
					job->run();
				}
				else {
					// we don't want to block the main thread for long jobs.
					--m_active;
					mainThreadSleepCv.notify_one();
					job->run();
					++m_active;
				}
				job->signalDependant();
			}
		}
		--m_active;
	});
}

void JobManager::wait() {
	processing = true;

	std::unique_lock<std::mutex> jl(jobQueueMutex);
	jobCv.notify_all();
	mainThreadSleepCv.wait(jl, [this]() {
		std::cout << "Main thread test." << std::endl;
		return numJobs() == 0 && m_active == 0;
	});
	processing = false;
}

std::unique_ptr<Job> JobManager::popJob() {
	std::unique_ptr<Job> res;
	while (!res && !exiting) {
		std::unique_lock<std::mutex> jl(jobQueueMutex);
		assert(jl);

		// Little bit messy doing this first but i don't want to reaquire the lock.
		if (numJobs() == 0) {
			--m_active;
			if (m_active == 0) {
				mainThreadSleepCv.notify_one();
			}
			jobCv.wait(jl, [this]() { return (processing && numJobs() > 0 && m_active < minThreads) || exiting; });
			++m_active;
		}

		if (numJobs() > 0 && !exiting) {  // frame locked jobs second.
			res = std::move(m_jobs[m_tail]);
			m_tail = (m_tail + 1) % maxJobs;
		}
	}
	return res;
}

void JobManager::notifyBlocked() {
	--m_active;
	jobCv.notify_one();
}

void JobManager::notifyUnblocked() {
	++m_active;
}

JobManager::~JobManager() {
	if (started) {
		exiting = true;
		jobCv.notify_all();
		for (int i = 0; i < maxThreads; ++i) {
			workerthreads[i].join();
		}
	}
}

void Job::addDependancy(Job* j) {
	this->remainingDependancies++;
	j->dependancies.emplace_back(this);
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
		dependanciesCV.wait(l, [this]() { return this->remainingDependancies == 0; });
		parent->notifyUnblocked();
	}
}