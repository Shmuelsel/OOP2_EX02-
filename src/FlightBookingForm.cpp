#include "FlightBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  
#include "config.h"

FlightBookingForm::FlightBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager) {  	
	initializeFields();
}
    
void FlightBookingForm::initializeFields() {

	fields.push_back(std::make_unique<Field<std::string>>("Departure Airport:"));
	fields.push_back(std::make_unique<Field<std::string>>("Arrival Airport:"));
	fields.push_back(std::make_unique<Field<std::string>>("Departure Date:", setDefaultDate()));
	//fields.push_back(std::make_unique<Field<std::string>>("Preferred Time:", "Don't Care"));
	
	std::vector<std::string> timeOptions = {
		"Morning",
		"Noon",
		"Evening",
		"Night",
		"Don't Care"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Preferred Time:", timeOptions, std::vector<std::string>{"Don't Care"},
		10, 480, 100, 30
	));
}

std::string FlightBookingForm::getFormType() const {
    return "Flight Booking Form";
}

void FlightBookingForm::render(sf::RenderWindow& window) {
}