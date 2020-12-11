#pragma once
#include <iostream>
#include <functional>

template <typename T>
void readData(T& data, const std::string& title, const std::string& error, std::function<bool(T)> checker) {
	bool isIncorrect{ true };
	do {
		std::cout << title;
		std::cin >> data;
		if (checker(data)) {
			isIncorrect = false;
		}
		else {
			std::cerr << error << std::endl;
		}
	} while (isIncorrect);
}