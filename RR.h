#pragma once
#include "Process.h"
#include "reader.h"
#include <vector>

class RR
{
private:

	static bool checkReady(std::vector<Process>& vec) {
		size_t ready{0};
		for (size_t i = 0; i < vec.size(); i++)
		{
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

		for (size_t i = 0; i < processCount; i++)
		{
			Process tmp(rand() % maxRunningTime, ((size_t)rand() % maxRunningTime + 1));
			vec.push_back(tmp);
			tmp.print(std::cout);
		}

	}

	static std::vector<Process>::iterator chooseNext(std::vector<Process>& vec, std::vector<Process>::iterator& iter) {
		
		bool findNew = false;

		for (auto it = iter; it != vec.end();it++) {
			if (it->getState() == Process::state_t::waiting) {
				findNew = true;
				return it;
			}
		}
		for (auto it = vec.begin(); it != iter; it++) {
			if (it->getState() == Process::state_t::waiting) {
				findNew = true;
				return it;
			}
		}
		if (!findNew) {
			return iter;
		}
	}

	static void startAlgorithm(std::vector<Process>& vec) {

		
		size_t ticks{ 0 };

		auto cur = vec.begin();

		const size_t ticksEach{ 2 };

		bool somethingWaiting = false;
	
		bool endOfCycle = false;

		while (!endOfCycle) {

		

			for (size_t i = 0; i < ticksEach; i++) {
				//Активация процесса по времени входа и добавление его в вектор активных процессов
			//РАБОТАЕТ
				for (auto it = vec.begin(); it != vec.end(); it++) {
					if (it->getAppearTime() == ticks) {
						it->activate();
						somethingWaiting = true;
					}
				}

				//Если есть хоть что-то в очереди активных процессов:
				if (somethingWaiting) {
					if (cur->getState() != Process::state_t::ready) {
						cur->run();

						//Остальные ждут
						for (auto it = vec.begin(); it != vec.end(); it++) {
							if (it != cur) {
								it->wait();
							}
						}

						/*if (cur->getState() == Process::state_t::ready || ticks % ticksEach == 0) {
							cur = chooseNext(vec, cur);
						}*/

						if (cur->getState() == Process::state_t::ready ||  (i+1) % ticksEach == 0) {
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

				if (checkReady(vec)) {
					endOfCycle = true;
				}
			}
			

		}

			std::cout << std::endl;
	}

	static void showResult(std::vector<Process>& vec) {

	}
};