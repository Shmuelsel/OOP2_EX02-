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
	fields.push_back(std::make_unique<Field<std::string>>("Hotel Name:", "", std::make_unique<NameValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Check-in Date:", setDefaultDate(), std::make_unique<DateValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Check-out Date:", setDefaultDate(), std::make_unique<DateValidator>()));
	fields.push_back(std::make_unique<Field<int>>("Number of Guests:", 0, std::make_unique<RangeValidator<int>>(1, 15)));

	std::vector<std::string> roomOptions = {
		"Single Room",
		"Double Room",
		"Family Room",
		"Presidential Suite"
	};

	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Room Type:", roomOptions, std::vector<std::string>{"Single Room"},
		std::make_unique<MultiChoiceValidator>(roomOptions),
		10, 520, 150, 30, false
	));
}

std::string HotelBookingForm::getFormType() const {
    return "Hotel Booking";
}

void HotelBookingForm::render(sf::RenderWindow& window) {
	//renderCommon(window);
}
void HotelBookingForm::handleInput(sf::Event event) {}

std::string HotelBookingForm::validateForm() const {
	std::string errorMessage;
	std::string checkInDate;
	std::string checkOutDate;
	for (const auto& field : fields) {
		if (field->getLabel() == "Check-in Date:") {
			checkInDate = field->getValueAsString();
		}
		else if (field->getLabel() == "Check-out Date:") {
			checkOutDate = field->getValueAsString();
		}
	}
	if (checkInDate >= checkOutDate) {
		return "Check-out date must be after check-in date.";
	}
	return errorMessage;

	//single room - 1 person, double room - 2 people, family room - any people, presidential suite - 6 people

	
}