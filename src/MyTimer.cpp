#include "MyTimer.h"


MyTimer::MyTimer() {
	current_time = 0;
	limit = 50;
}

MyTimer::MyTimer(unsigned int l) {
	current_time = 0;
	limit = l;
}

MyTimer::~MyTimer(){

}

//decrease current time
void MyTimer::tick(){
	if (current_time > 0) current_time--;
}

//set timer limit (length)
void MyTimer::setLimit(unsigned int l){
	limit = l;
}

//reset timer back to limit 
void MyTimer::reset() {
	current_time = limit;
}

//check if timer is up
bool MyTimer::ready(){
	return (current_time == 0);
}


