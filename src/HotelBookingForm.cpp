#include "HotelBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date
#include "config.h"
#include "Field.h"


HotelBookingForm::HotelBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager)  {  // ✅ Calls base constructor
    fieldLabels.insert(fieldLabels.end(), Config::fieldLabelsHotel.begin(), Config::fieldLabelsHotel.end());

    fields.push_back(std::make_unique<Field<std::string>>("Hotel Name:"));
    fields.push_back(std::make_unique<Field<std::string>>("Check-in Date:", setDefaultValues()));
	fields.push_back(std::make_unique<Field<std::string>>("Check-out Date:", setDefaultValues()));
	fields.push_back(std::make_unique<Field<int>>("Number of Guests:"));
	fields.push_back(std::make_unique<Field<std::string>>("Room Type:", "Single Room"));

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

void HotelBookingForm::render(sf::RenderWindow& window) {
	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");
     //✅ Render input fields dynamically
    int yOffset = 60;
    float roomTypeButtonX = 10;
    for (int i = 0; i < roomTypeSelection.size(); ++i) {
        sf::RectangleShape roomButton(sf::Vector2f(150, 30));
        roomButton.setPosition(roomTypeButtonX, yOffset);
        roomButton.setFillColor(selectedRoomType==i ? sf::Color(0, 120, 255) : sf::Color::White);  // ✅ Highlight selected
        roomButton.setOutlineThickness(2);
        roomButton.setOutlineColor(sf::Color(160, 160, 160));
        window.draw(roomButton);

        sf::Text roomText(roomTypeSelection[i], font, 16);
        roomText.setFillColor(selectedRoomType == i ? sf::Color::White : sf::Color::Black);
        roomText.setPosition(roomTypeButtonX + 10, yOffset + 5);
        window.draw(roomText);

        roomTypeButtonX += 160;  // ✅ Increased spacing
    }
    

}

void HotelBookingForm::handleInput(sf::Event event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !userInput[activeField].empty()) {
            userInput[activeField].pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            userInput[activeField] += static_cast<char>(event.text.unicode);
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            activeField = (activeField + 1) % userInput.size();
        }
        if (event.key.code == sf::Keyboard::Return) {
            std::cout << "Entered Data: ";
            for (const auto& field : userInput) std::cout << field << " ";
            std::cout << std::endl;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        int yOffset = 60;  // ✅ Start from the top

        for (std::size_t i = 0; i < fieldLabels.size(); ++i) {
            sf::FloatRect inputBoxBounds(260, yOffset - 5, 250, 35);  // ✅ Uses dynamic positioning
            if (inputBoxBounds.contains(mousePos)) {
                activeField = i;
                return;
            }
            yOffset += 50;  // ✅ Move down dynamically (same logic as `render()`)
        }


        // ✅ Handle Room Type Button Click          
        float roomButtonX = 10;
                for (int i = 0; i < roomTypeSelection.size(); ++i) {
            sf::FloatRect timeButtonBounds(roomButtonX, yOffset, 150, 30);
            if (timeButtonBounds.contains(mousePos)) {
				selectedRoomType = i;
                //Update the "Room Type" input box
                userInput[8] = roomTypeSelection[i];             

                return;
            }
            roomButtonX += 160;  // Move to next button
        }

        //================================================================================================
		// can be deleted
        //// ✅ Handle "Done" Button Click
        //if (mousePos.x >= 20 && mousePos.x <= 160 && mousePos.y >= 570 && mousePos.y <= 610) {
        //    std::cout << "Flight Booking Confirmed!\n";
        //    openConfirmationWindow();  // ✅ Open confirmation
        //    return;
        //}

        //// ✅ Handle "Cancel" Button Click
        //if (mousePos.x >= 200 && mousePos.x <= 340 && mousePos.y >= 570 && mousePos.y <= 610) {
        //    std::cout << "Cancelled Flight Booking\n";
        //    formManager->closeForm();
        //    return;
        //}
				//================================================================================================
    }
}

const std::vector<std::unique_ptr<FieldBase>>& HotelBookingForm::getFields() const{
    return fields; // מחזיר רפרנס לווקטור
}