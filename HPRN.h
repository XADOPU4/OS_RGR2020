#pragma once
#include "Process.h"
#include "reader.h"
#include <algorithm>
#include <vector>


class HPRN {

private:

	static bool checkReady(std::vector<Process>& vec) {
		size_t ready{ 0 };
		for (size_t i = 0; i < vec.size(); i++) {
			if (vec[i].getState() == Process::state_t::ready) {
				ready++;
			}
		}

		return ready == vec.size();
	}

public:

	static void init(std::vector<Process>& vec) {

		int processCount{ 0 };
		readData<int>(processCount, "Enter the quantity of processes: ", "Invalid value, try again!", [](int a) {return a > 0; });

		int maxRunningTime{ 0 };
		readData<int>(maxRunningTime, "Enter maximum processing time: ", "Invalid value, try again!", [](int a) {return a > 0; });

		for (size_t i = 0; i < processCount; i++) {
			Process tmp(rand() % maxRunningTime, ((size_t)rand() % maxRunningTime + 1));
			vec.push_back(tmp);
			tmp.print(std::cout);
		}

	}

	static std::vector<Process>::iterator chooseNext(std::vector<Process>& vec, std::vector<Process>::iterator& iter) {

		bool findNew = false;
		float penaltyRatio = 0.f;
		float maxPenaltyRatio = 0.f;
		

		std::vector<Process>::iterator tmp = vec.begin();

		for (auto it = vec.begin(); it != vec.end(); it++) {
			penaltyRatio = (it->getLostTime() + it->getProcessingTime()) / it->getProcessingTime();
			if (it->getState() == Process::state_t::waiting) {
				if (penaltyRatio > maxPenaltyRatio) {
					maxPenaltyRatio = penaltyRatio;
				}
			}
			it->setPenaltyRatio(penaltyRatio);
		}

		for (auto it = vec.begin(); it != vec.end(); it++) {
			if (it->getState() != Process::state_t::ready && it->getState() != Process::state_t::inactive) {
				if (it->getPenaltyRatio() == maxPenaltyRatio) {
					return it;
				}
			}
		}

		return iter;
		
	}

	static void startAlgorithm(std::vector<Process>& vec) {


		size_t ticks{ 0 };

		auto cur = vec.begin();

		bool somethingWaiting = false;

		bool endOfCycle = false;

		while (!endOfCycle) {

			//Активация процесса по времени входа
			for (auto it = vec.begin(); it != vec.end(); it++) {
				if (it->getAppearTime() == ticks) {
					it->activate();
					if (somethingWaiting == false) {
						cur = chooseNext(vec, cur);
					}
					somethingWaiting = true;
				}
			}

			//Если есть хоть что-то в очереди активных процессов:
			if (somethingWaiting) {

				if (cur->getState() == Process::state_t::ready) {
					cur = chooseNext(vec, cur);
				}

				if (cur->getState() != Process::state_t::ready) {
					cur->run();

					//Остальные ждут
					for (auto it = vec.begin(); it != vec.end(); it++) {
						if (it != cur) {
							it->wait();
						}
					}
				}

				//Если выполнился на этом шаге, то выбираем другой процесс для исполнения
				if (cur->getState() == Process::state_t::ready) {

					if (checkReady(vec)) {
						endOfCycle = true;
					}
					else {
						cur = chooseNext(vec, cur);
					}
				}
			}



			//Печать таблицы
			std::cout << ticks << ".";
			for (size_t i = 0; i < vec.size(); i++) {

				vec[i].printFull(std::cout, vec[i].getState());
			}
			std::cout << std::endl;


			ticks++;

			
		}

		std::cout << std::endl;
	}

	static void showResult(std::vector<Process>& vec) {

	}
};

