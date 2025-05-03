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

	fields.push_back(std::make_unique<Field<std::string>>("Departure Airport:", "", std::make_unique<NameValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Arrival Airport:", "", std::make_unique<NameValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Departure Date:", setDefaultDate(), std::make_unique<DateValidator>()));

	std::vector<std::string> preferredTimeOptions = {
		"Morning",
		"Noon",
		"Evening",
		"Night",
		"Don't Care"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Preferred Time:", preferredTimeOptions, std::vector<std::string>{"Don't Care"},
		std::make_unique<MultiChoiceValidator>(preferredTimeOptions),
		10, 520, 150, 30
	));
}

std::string FlightBookingForm::getFormType() const {
    return "Flight Booking Form";
}

void FlightBookingForm::render(sf::RenderWindow& window) {
}

std::string FlightBookingForm::validateForm() const {
	std::string errorMessage;
	std::string depAirport;
	std::string arrAirport;
	for (const auto& field : fields) {
		if (field->getLabel() == "Departure Airport:") {
			depAirport = field->getValueAsString();
		}
		else if (field->getLabel() == "Arrival Airport:") {
			arrAirport = field->getValueAsString();
		}
	}

	if (depAirport == arrAirport) {
		return "Departure and Arrival airports cannot be the same.";
	}
}