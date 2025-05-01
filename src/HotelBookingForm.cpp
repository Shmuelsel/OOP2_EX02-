#include "HotelBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>
#include "config.h"
#include "Field.h"


HotelBookingForm::HotelBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager)  { 
    fieldLabels.insert(fieldLabels.end(), Config::fieldLabelsHotel.begin(), Config::fieldLabelsHotel.end());

	initializeFields();

	/*selectionButtons.push_back(Button("Single Room", 10, 520, 150, 30));
	selectionButtons.push_back(Button("Double Room", 170, 520, 150, 30));
	selectionButtons.push_back(Button("Family Room", 330, 520, 150, 30));
	selectionButtons.push_back(Button("Presidential Suite", 490, 520, 150, 30));*/
}

void HotelBookingForm::initializeFields() {
	fields.push_back(std::make_unique<Field<std::string>>("Hotel Name:"));
	fields.push_back(std::make_unique<Field<std::string>>("Check-in Date:", setDefaultValues()));
	fields.push_back(std::make_unique<Field<std::string>>("Check-out Date:", setDefaultValues()));
	fields.push_back(std::make_unique<Field<int>>("Number of Guests:"));

	std::vector<std::string> roomOptions = {
		"Single Room",
		"Double Room",
		"Family Room",
		"Presidential Suite"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Room Type:", roomOptions, std::vector<std::string>{"Single Room"},
		10, 520, 150, 30
	));
}

std::string HotelBookingForm::setDefaultValues() {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);  
	return std::to_string(1900 + ltm.tm_year) + "-" +
		std::to_string(1 + ltm.tm_mon) + "-" +
		std::to_string(ltm.tm_mday);
}

std::string HotelBookingForm::getFormType() const {
    return "Hotel Booking";
}

void HotelBookingForm::render(sf::RenderWindow& window) {}
void HotelBookingForm::handleInput(sf::Event event) {}