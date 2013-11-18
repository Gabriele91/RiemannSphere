#ifndef TIMER_H
#define TIMER_H

#include <Config.h>
#include <Types.h>
#include <ETime.h>

namespace Easy3D {

	class Timer {

		bool started;
		double sTime;

	public:

		Timer():started(false),sTime(0.0){}
		void start();
		void start(double stime);
		double getGetCounter();
		void reset();
		void end();

	};

};

#endif