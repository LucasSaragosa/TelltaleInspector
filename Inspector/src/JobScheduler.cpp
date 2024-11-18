#include "TelltaleInspector.h"
#include <algorithm>

// == SET THREAD NAME WINDOWS HELPER. SEE 'TIPS FOR DEBUGGING THREADS' ON MSVC WEBSITE.

#ifdef _MSC_VER

static void SetThreadNameA(DWORD id, const char* pName){
#pragma pack(push, 8)
	struct {
		DWORD dwType;
		LPCSTR szName;
		DWORD dwThreadID;
		DWORD dwFlags;
	} info{};
#pragma pack(pop)
#pragma warning(push)
#pragma warning(disable : 6320 6322)
	info.dwType = 0x1000;
	info.dwThreadID = id;
	info.szName = pName;
	info.dwFlags = 0;
	__try {
		RaiseException(0X406d1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}__except(EXCEPTION_EXECUTE_HANDLER) {}
#pragma warning(pop)
}

#else

static void SetThreadNameA(u32, const char*) {}

#endif

// === SIMPLE JOB SCHEDULER ===

JobScheduler* JobScheduler::mpScheduler{NULL};

JobScheduler::JobScheduler()
{
	sem = CreateSemaphoreA(0, 0, LONG_MAX, 0);
	std::atomic_bool* pg = new std::atomic_bool(true);
	for (int i = 0; i < 6; i++) {
		threadPool.push_back(std::thread(ThreadProc, this, i, pg));
	}
	pRunningGuard = pg;
}

JobScheduler::~JobScheduler()
{
	{
		lock.lock();
		while (pendingJobs.size()) {
			if (pendingJobs.top().priority >= PRIORITY_DISALLOW_TASK_EXIT) {
				lock.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				TTL_Log("Tasks are taking long time to complete (and are highest priorities!). Wait loop completed\n");
				lock.lock();
			}
			pendingJobs.pop();
		}
		lock.unlock();
	}
	pRunningGuard->store(false);
	for (auto& thread : threadPool)
		thread.join();
	delete pRunningGuard;
	threadPool.clear();
}

bool InspectorTask::HasAsyncDependencies(){
	bool bRemoved = false;
	for (;;) {
		int i = 0;
		for (auto& job : mSubmittedJobs) {
			if (JobScheduler::Scheduler().Finished(job)) {
				bRemoved = true;
				auto it = mSubmittedJobs.begin();
				std::advance(it, i);
				mSubmittedJobs.erase(it);
				break;
			}
			i++;
		}
		if (!bRemoved)
			break;
		bRemoved = false;
	}
	return mSubmittedJobs.size() != 0;
}

int JobScheduler::PostJob(InspectorTask* pTask, Job job)
{
	int handle;
	{
		std::lock_guard<std::mutex> _l(lock);
		handle = job.handle = runningHnd++;
		pTask->mSubmittedJobs.push_back(handle);
		pendingJobs.push(std::move(job));
		aliveJobs.push_back(handle);
	}
	ReleaseSemaphore(sem, 1, 0);
	return handle;
}

bool JobScheduler::Finished(int handle){
	std::lock_guard<std::mutex> _l{ lock };
	for (auto it : aliveJobs)
		if (it == handle)
			return false;
	return true;
}

void JobScheduler::ThreadProc(JobScheduler* me, int threadIndex, std::atomic_bool* pRunning)
{
	char tmp[32];
	char s[2]{ 'a',0 };
	s[0] += threadIndex;
	sprintf(tmp, "%u%s", 0, s);
	DWORD curThread = GetCurrentThreadId();//platformmulti fix maybe
	SetThreadNameA(curThread, tmp);
	TTL_Log("Thread %s[0x%X] start has signaled\n", tmp, curThread);
	while (pRunning->load()) {
		if (WaitForSingleObject(me->sem, 100) == WAIT_OBJECT_0) {
			Job job{};
			{
				std::lock_guard<std::mutex> _l(me->lock);
				job = me->pendingJobs.top();
				me->pendingJobs.pop();
			}
			if (job.name.length() > 0)
				TTL_Log("Executing scheduled job: '%s' on thread JobThread %s [thread 0x%X]\n", job.name.c_str(), tmp, curThread);
			job.mpFn(job.mpUserInstance, job.userA, job.userB);
			{
				std::lock_guard<std::mutex> _l(me->lock);
				me->aliveJobs.erase(std::remove(me->aliveJobs.begin(), me->aliveJobs.end(), job.handle),me->aliveJobs.end());
			}
		}
	}
	TTL_Log("Thread %s[0x%X] has joined\n", tmp, curThread);
}
