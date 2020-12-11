#include "Process.h"

void Process::run()
{
	if (this->state != state_t::inactive && this->state != state_t::ready) {
		this->state = state_t::processing;
		this->systemTime++;
		if (this->systemTime - this->lostTime == this->processingTime) {
			this->state = state_t::ready;
		}
	}
}

void Process::wait()
{
	if (this->state != state_t::inactive && this->state != state_t::ready) {
		this->state = state_t::waiting;
		this->lostTime++;
		this->systemTime++;
	}
}

void Process::activate()
{
	if (this->state == state_t::inactive) {
		this->state = state_t::waiting;
	}

}
	
