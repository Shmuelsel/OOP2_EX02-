#include "FlightBookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date
#include "config.h"

FlightBookingForm::FlightBookingForm(sf::RenderWindow& win, DialogueManager* manager)
    : BookingForm(win, manager) {  // ✅ Calls base constructor
    fieldLabels.insert(fieldLabels.end(), Config::fieldLabelsFlight.begin(), Config::fieldLabelsFlight.end());
	
    fields.push_back(std::make_unique<Field<std::string>>("Departure Airport:"));
    fields.push_back(std::make_unique<Field<std::string>>("Arrival Airport:"));
    fields.push_back(std::make_unique<Field<std::string>>("Departure Date:", setDefaultValues()));
    fields.push_back(std::make_unique<Field<std::string>>("Preferred Time:", "Don't Care"));

    //userInput.resize(fields.size(), "");  // Resize to include all fields
    //setDefaultValues();
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

void FlightBookingForm::render(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");
    //✅ Render input fields dynamically
    int yOffset = 60;
    // ✅ Time Selection Buttons 
    float timeButtonX = 10;
    for (std::size_t i = 0; i < timeSelection.size(); ++i) {
        sf::RectangleShape timeButton(sf::Vector2f(100, 30));
        timeButton.setPosition(timeButtonX, yOffset);
        timeButton.setFillColor(timeSelection[i].second ? sf::Color(0, 120, 255) : sf::Color::White);
        timeButton.setOutlineThickness(2);
        timeButton.setOutlineColor(sf::Color(160, 160, 160));
        window.draw(timeButton);

        sf::Text timeText(timeSelection[i].first, font, 16);
        timeText.setFillColor(timeSelection[i].second ? sf::Color::White : sf::Color::Black);
        timeText.setPosition(timeButtonX + 10, yOffset+2);
        window.draw(timeText);

        timeButtonX += 110;  // ✅ Increased spacing between buttons
    }
}

const std::vector<std::unique_ptr<FieldBase>>& FlightBookingForm::getFields() const{
    return fields; // מחזיר רפרנס לווקטור
}

//void FlightBookingForm::handleInput(sf::Event event) {
//    if (event.type == sf::Event::TextEntered) {
//        if (event.text.unicode == '\b' && !userInput[activeField].empty()) { 
//            userInput[activeField].pop_back();  // ✅ Handle Backspace
//        } 
//        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
//            userInput[activeField] += static_cast<char>(event.text.unicode);  // ✅ Append typed character
//        }
//    } 
//    else if (event.type == sf::Event::KeyPressed) {
//        if (event.key.code == sf::Keyboard::Tab) {
//            activeField = (activeField + 1) % userInput.size();  // ✅ Move to next input field
//        }
//        if (event.key.code == sf::Keyboard::Return) {
//            std::cout << "Entered Data: ";
//            for (const auto& field : userInput) std::cout << field << " ";
//            std::cout << std::endl;
//        }
//    }
//    else if (event.type == sf::Event::MouseButtonPressed) {
//        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
//
//        int yOffset = 60;  // ✅ Start from the top
//
//        for (std::size_t i = 0; i < fieldLabels.size(); ++i) {
//            sf::FloatRect inputBoxBounds(260, yOffset - 5, 250, 35);  // ✅ Uses dynamic positioning
//            if (inputBoxBounds.contains(mousePos)) {
//                activeField = i;
//                return;
//            }
//            yOffset += 50;  // ✅ Move down dynamically (same logic as `render()`)
//        }
//
//
//        // ✅ Handle Time Selection Button Click          
//        float timeButtonX = 10;
//               
//        for (int i = 0; i < 5; ++i) {           
//            sf::FloatRect timeButtonBounds(timeButtonX, yOffset, 100, 30);
//            if (timeButtonBounds.contains(mousePos)) {                
//                timeSelection[i].second = !timeSelection[i].second;
//                //Update the "Preferred Time" input box
//                userInput[7] = "";
//                for (int i = 0; i < 5; ++i) {
//                    if (timeSelection[i].second) {
//                        if (!userInput[7].empty()) userInput[7] += "| ";  // ✅ Separate multiple selections
//                        userInput[7] += timeSelection[i].first;
//                    }
//                }
//
//                return;
//            }
//            timeButtonX += 110;  // Move to next button
//            
//            
//        }
//    
//		//================================================================================================
//		// can be deleted
//        // ✅ Handle "Done" Button Click
//        //if (mousePos.x >= 20 && mousePos.x <= 160 && mousePos.y >= 550 && mousePos.y <= 590) {
//        //    std::cout << "Flight Booking Confirmed!\n";
//        //    openConfirmationWindow();  // ✅ Open confirmation
//        //    return;
//        //}
//
//        //// ✅ Handle "Cancel" Button Click
//        //if (mousePos.x >= 200 && mousePos.x <= 340 && mousePos.y >= 550 && mousePos.y <= 590) {
//        //    std::cout << "Cancelled Flight Booking\n";
//        //    formManager->closeForm();
//        //    return;
//        //}
//		//================================================================================================
//    }
//
//}






