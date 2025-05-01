#include "UIManager.h"
#include <iostream>
#include "FlightBookingForm.h"
#include "HotelBookingForm.h"
#include "CarRentalForm.h"
#include "EventBookingForm.h"
#include "TrainBookingForm.h"
#include "Button.h"

UIManager::UIManager(sf::RenderWindow& win, DialogueManager& dManager)
    : window(win), formManager(dManager) {
	initializeButtons();
}

void UIManager::initializeButtons() {
    font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");

    buttons.push_back(Button("Flight Booking", 100, 150, 300, 50));
    buttons.push_back(Button("Hotel Booking", 100, 220, 300, 50));
    buttons.push_back(Button("Car Rental", 100, 290, 300, 50));
    buttons.push_back(Button("Event Booking", 100, 360, 300, 50));
    buttons.push_back(Button("Train Booking", 100, 430, 300, 50));
}

void UIManager::render() {
    sf::Text title("Travel Booking System", font, 30);
    title.setFillColor(sf::Color::Black);
    title.setStyle(sf::Text::Bold);
    title.setPosition(80, 20);
    window.draw(title);

    for (const auto& button : buttons) {
        button.render(window, font);
    }
}

void UIManager::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        for (auto& button : buttons) {
            if (button.handleClick(mousePos)) {
                if (button.getText() == "Flight Booking") {
                    formManager.setActiveForm(std::make_unique<FlightBookingForm>(window, &formManager));
                }
                else if (button.getText() == "Hotel Booking") {
                    formManager.setActiveForm(std::make_unique<HotelBookingForm>(window, &formManager));
                }
                else if (button.getText() == "Car Rental") {
                    formManager.setActiveForm(std::make_unique<CarRentalForm>(window, &formManager));
                }
                else if (button.getText() == "Event Booking") {
                    formManager.setActiveForm(std::make_unique<EventBookingForm>(window, &formManager));
                }
                else if (button.getText() == "Train Booking") {
                    formManager.setActiveForm(std::make_unique<TrainBookingForm>(window, &formManager));
                }
            }
        }
    }
}
