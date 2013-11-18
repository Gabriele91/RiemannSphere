#include <stdafx.h>
#include <Timer.h>

using namespace Easy3D;

void Timer::start(){
	started=true;
	sTime=GetTime();
}
void Timer::start(double stime){
	started=true;
	sTime=stime;
}
double Timer::getGetCounter(){
	return started ? GetTime()-sTime : 0;
}
void Timer::reset(){
	sTime=GetTime();
}
void Timer::end(){
	started=false;
	sTime=0;
}