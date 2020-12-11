#pragma once
#include <iomanip>
#include <iostream>



class Process
{

public:
	enum class state_t {
		inactive, processing, waiting, ready
	};

private:
	size_t appearTime;
	size_t systemTime;
	size_t lostTime;
	size_t processingTime;
	float penaltyRatio;
	state_t state = state_t::inactive;

public:
	Process() :appearTime(0), processingTime(0), systemTime(0), lostTime(0),penaltyRatio(0.f) {}
	Process(size_t appearTime, size_t processingTime) : appearTime(appearTime), processingTime(processingTime), systemTime(0), lostTime(0), penaltyRatio(0.f) {}
	
	void run();
	void wait();
	void activate();

	void setPenaltyRatio(float value) { this->penaltyRatio = value; }
	float getPenaltyRatio() { return this->penaltyRatio; }

	size_t getAppearTime() { return this->appearTime; }
	size_t getSystemTime() { return this->systemTime; }
	size_t getLostTime() { return this->lostTime; }
	size_t getProcessingTime() { return this->processingTime; }
	state_t getState() { return this->state; }

	void print(std::ostream& out) { out << "Appear Time: " << this->appearTime << " Required Processing Time: " << this->processingTime << std::endl; }
	void printFull(std::ostream& out, state_t state) {
		switch (state)
		{
		case state_t::inactive:
			out << std::setw(5) << "X ";
			break;
		case state_t::processing:
			out << std::setw(5) << "P ";
			break;
		case state_t::waiting:
			out << std::setw(5) << "W ";
			break;
		case state_t::ready:
			out << std::setw(5) << "R ";
			break;
		}
	}
};

