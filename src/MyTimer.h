#pragma once

class MyTimer {
private:
	unsigned int current_time, limit;

public:
	MyTimer();
	MyTimer(unsigned int l);
	~MyTimer();

	void setLimit(unsigned int l);

	void tick();
	void reset();

	bool ready();
};