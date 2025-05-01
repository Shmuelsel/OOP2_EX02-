#include "FlightBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  
#include "config.h"

FlightBookingForm::FlightBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager) {  
    //fieldLabels.insert(fieldLabels.end(), Config::fieldLabelsFlight.begin(), Config::fieldLabelsFlight.end());
	
	initializeFields();
}
    
void FlightBookingForm::initializeFields() {

	fields.push_back(std::make_unique<Field<std::string>>("Departure Airport:"));
	fields.push_back(std::make_unique<Field<std::string>>("Arrival Airport:"));
	fields.push_back(std::make_unique<Field<std::string>>("Departure Date:", setDefaultValues()));
	fields.push_back(std::make_unique<Field<std::string>>("Preferred Time:", "Don't Care"));
	
	std::vector<std::string> timeOptions = {
		"Morning",
		"Noon",
		"Evening",
		"Night",
		"Don't Care"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Preferred Time:", timeOptions, std::vector<std::string>{"Don't Care"},
		10, 520, 150, 30
	));
}


std::string FlightBookingForm::setDefaultValues() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);  // ✅ Safe alternative to localtime()
    return std::to_string(1900 + ltm.tm_year) + "-" +
        std::to_string(1 + ltm.tm_mon) + "-" +
        std::to_string(ltm.tm_mday);
}

std::string FlightBookingForm::getFormType() const {
    return "Flight Booking Form";
}

void FlightBookingForm::render(sf::RenderWindow& window) { }






