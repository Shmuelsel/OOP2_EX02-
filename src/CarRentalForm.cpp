#include "CarRentalForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date

CarRentalForm::CarRentalForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager) {  
	initializeFields();
}

void CarRentalForm::initializeFields() {
    fields.push_back(std::make_unique<Field<std::string>>("Name:"));
    fields.push_back(std::make_unique<Field<std::string>>("ID:"));
    fields.push_back(std::make_unique<Field<std::string>>("Address:"));
    fields.push_back(std::make_unique<Field<std::string>>("Email:"));
    fields.push_back(std::make_unique<Field<std::string>>("Pick-up Date:", setDefaultDate()));
    fields.push_back(std::make_unique<Field<std::string>>("Drop-off Date:", setDefaultDate()));

    std::vector<std::string> carTypeOptions = {
        "Economy",
        "Compact",
        "SUV",
        "Luxury"
    };

    fields.push_back(std::make_unique<Field<std::vector<std::string>>>(
        "Car Type:", carTypeOptions, std::vector<std::string>{"Economy"},
        10, 520, 130, 30
    ));
}

std::string CarRentalForm::getFormType() const {
    return "Car Rental";
}

void CarRentalForm::render(sf::RenderWindow& window) {}
