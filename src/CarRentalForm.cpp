#include "CarRentalForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date

CarRentalForm::CarRentalForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager) {  
	initializeFields();
}

void CarRentalForm::initializeFields() {
    
	fields.push_back(std::make_unique<Field<std::string>>("Name:", "", std::make_unique<NameValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("ID:", "", std::make_unique<IDValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Address:", "", std::make_unique<AddressValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Email:", "", std::make_unique<EmailValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Pick-up Date:", setDefaultDate(), std::make_unique<DateValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Drop-off Date:", setDefaultDate(), std::make_unique<DateValidator>()));
	std::vector<std::string> carTypeOptions = {
		"Economy",
		"Compact",
		"SUV",
		"Luxury"
	};
	fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
		"Car Type:", carTypeOptions, std::vector<std::string>{"Economy"},
		std::make_unique<MultiChoiceValidator>(carTypeOptions),
		10, 520, 130, 30
	));

}

std::string CarRentalForm::getFormType() const {
    return "Car Rental";
}

void CarRentalForm::render(sf::RenderWindow& window) {}

std::string CarRentalForm::validateForm() const {
	std::string errorMessage;
	std::string pickUpDate;
	std::string dropOffDate;
	for (const auto& field : fields) {
		if (field->getLabel() == "Pick-up Date:") {
			pickUpDate = field->getValueAsString();
		}
		else if (field->getLabel() == "Drop-off Date:") {
			dropOffDate = field->getValueAsString();
		}
	}
	if (pickUpDate >= dropOffDate) {
		return "Drop-off date must be after pick-up date.";
	}
	return errorMessage;
}
