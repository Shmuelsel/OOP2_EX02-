// DatabaseManager.cpp - Implements Data Storage
#include "DatabaseManager.h"
#include <iostream>
#include <fstream>

void DatabaseManager::saveBookingData(const std::string& bookingData) {
	std::ofstream outFile("bookings.txt", std::ios::app);
	if (outFile.is_open()) {
		outFile << bookingData << std::endl;
		outFile.close();
	}
	else {
		std::cerr << "Error opening file for writing." << std::endl;
	}

}

std::vector<std::string> DatabaseManager::loadBookings() {
	std::vector<std::string> bookings;
	std::ifstream inFile("bookings.txt");
	if (inFile.is_open()) {
		std::string line;
		while (std::getline(inFile, line)) {
			bookings.push_back(line);
		}
		inFile.close();
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	return bookings;
}
