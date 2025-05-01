#include "HotelBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>
#include "config.h"
#include "Field.h"

HotelBookingForm::HotelBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager)  { 
	initializeFields();
}

void HotelBookingForm::initializeFields() {
	fields.push_back(std::make_unique<Field<std::string>>("Hotel Name:"));
	fields.push_back(std::make_unique<Field<std::string>>("Check-in Date:", setDefaultDate()));
	fields.push_back(std::make_unique<Field<std::string>>("Check-out Date:", setDefaultDate()));
	fields.push_back(std::make_unique<Field<int>>("Number of Guests:"));

	std::vector<std::string> roomOptions = {
		"Single Room",
		"Double Room",
		"Family Room",
		"Presidential Suite"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Room Type:", roomOptions, std::vector<std::string>{"Single Room"},
		10, 520, 130, 30
	));
}

std::string HotelBookingForm::getFormType() const {
    return "Hotel Booking";
}

void HotelBookingForm::render(sf::RenderWindow& window) {
	//renderCommon(window);
}
void HotelBookingForm::handleInput(sf::Event event) {}