#ifndef POOLTHREAD_H
#define POOLTHREAD_H

#include <Easy3D.h>
namespace RiemannSphere {

	class PoolThread {
		
		bool dotask;
		Easy3D::Mutex mlist;
		std::list< DFUNCTION< void (void) > > tasks;
		std::vector< Easy3D::Thread  > workers;

	public:

		PoolThread(Easy3D::uint nworkers=1);

		void claerAllTask(){
			mlist.lock();
				tasks.clear();
			mlist.unlock();
		}
		void addTaskBack(const DFUNCTION< void (void) >& task){
			mlist.lock();
				tasks.push_back(task);
			mlist.unlock();
		}
		void addTaskFront(const DFUNCTION< void (void) >& task){
			mlist.lock();
				tasks.push_front(task);
			mlist.unlock();
		}

		virtual ~PoolThread(){
			dotask=false;
			for(auto& worker:workers)
				worker.join();
		}

	};

};

#endif