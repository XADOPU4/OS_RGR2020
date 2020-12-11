#include "RR.h"
#include "HPRN.h"
#include <ctime>
#include <iostream>


int main() {

	srand(time(NULL));
	std::vector<Process> vec;

	int choice{ 0 };
	readData<int>(choice, "1.RR \n2.HPRN", "Invalid value, try again!", [](int a) {return a > 0 && a < 3; });

	switch (choice)
	{
	case 1:
		RR::init(vec);
		RR::startAlgorithm(vec);
		RR::showResult(vec);
		break;
	case 2:
		HPRN::init(vec);
		HPRN::startAlgorithm(vec);
		HPRN::showResult(vec);
		break;

	default:
		break;
	}

	return 1;
}