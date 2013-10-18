#include <stdafx.h>
#include <PoolThread.h>

///////////////////////
using namespace Easy3D;
using namespace RiemannSphere;
///////////////////////

PoolThread::PoolThread(uint nworkers):workers(nworkers){
	dotask=true;
	while(nworkers--){
		Easy3D::Thread* worker=&workers[nworkers];
		workers[nworkers].setArgs(NULL);
		workers[nworkers].setFunction(
		[this,worker](void*)->int{
			//task
			DFUNCTION< void(void) > task;
			//loop
			while(dotask){
				//disable last task
			    task=nullptr;
				//critical sections
				mlist.lock();
				if(tasks.begin()!=tasks.end()){
					//get task
					task = (*tasks.begin());
					//pop task
					tasks.pop_front();
				}
				mlist.unlock();
				//call
				if(task) task();
				//sleep thread
                else worker->sleepThread(16);
			}
			return 0;
		});
		workers[nworkers].start();
	}
}
	