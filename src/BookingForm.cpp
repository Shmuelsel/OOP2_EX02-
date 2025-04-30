#include "BookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date
#include "config.h"
#include "Field.h"
#include "Button.h"

BookingForm::BookingForm(sf::RenderWindow& win, DialogueManager* manager) :window(win), formManager(manager) {
    //fieldLabels = Config::fieldLabelsBase;  // ✅ Add common fields
	
    fields.push_back(std::make_unique<Field<std::string>>("Name:"));
    fields.push_back(std::make_unique<Field<std::string>>("ID:"));
    fields.push_back(std::make_unique<Field<std::string>>("Address:"));
    fields.push_back(std::make_unique<Field<std::string>>("Email:"));

	buttons.push_back(Button("Done", 20, 570, 140, 40));
	buttons.push_back(Button("Cancel", 200, 570, 140, 40));
    //userInput.resize(fields.size(), "");  // Initialize input fields
}

void BookingForm::openConfirmationWindow() {
    const std::string& formTitle = getFormType();
    sf::RenderWindow confirmWindow(sf::VideoMode(500, 600), "Confirm " + formTitle);

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");

    bool approved = false;

    while (confirmWindow.isOpen()) {
        sf::Event event;
        while (confirmWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                confirmWindow.close();

            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            if (event.type == sf::Event::MouseButtonPressed) {
                if (mousePos.x >= 280 && mousePos.x <= 400 && mousePos.y >= 300 && mousePos.y <= 345) {
                    confirmWindow.close();
                }
                if (mousePos.x >= 100 && mousePos.x <= 220 && mousePos.y >= 300 && mousePos.y <= 345) {
                    std::cout << formTitle << " Confirmed! Returning to main menu." << std::endl;
                    approved = true;
                    confirmWindow.close();
                }
            }
        }

        confirmWindow.clear(sf::Color(240, 240, 240));

        sf::Text title("Confirm " + formTitle, font, 22);
        title.setFillColor(sf::Color::Black);
        title.setStyle(sf::Text::Bold);
        title.setPosition(130, 20);
        confirmWindow.draw(title);

        std::string bookingInfo;
        for (size_t i = 0; i < fields.size(); ++i) {
            bookingInfo += fields[i]->getLabel() + " " + fields[i]->getValueAsString() + "\n";
        }

        sf::Text details(bookingInfo, font, 18);
        details.setFillColor(sf::Color::Black);
        details.setPosition(50, 80);
        confirmWindow.draw(details);


        // ✅ Approve Button
        //sf::RectangleShape approveButton(sf::Vector2f(120, 40));
        //approveButton.setPosition(100, 300);
        //approveButton.setFillColor(sf::Color(50, 150, 50));
        //confirmWindow.draw(approveButton);

        //sf::Text approveText("APPROVE", font, 18);
        //approveText.setFillColor(sf::Color::White);
        //approveText.setPosition(118, 310);
        //confirmWindow.draw(approveText);

        //// ✅ Cancel Button
        //sf::RectangleShape cancelButton(sf::Vector2f(120, 40));
        //cancelButton.setPosition(280, 300);
        //cancelButton.setFillColor(sf::Color(180, 0, 0));
        //confirmWindow.draw(cancelButton);

        //sf::Text cancelText("CANCEL", font, 18);
        //cancelText.setFillColor(sf::Color::White);
        //cancelText.setPosition(305, 310);
        //confirmWindow.draw(cancelText);

        //confirmWindow.display();
		
    }

    if (approved) {
        formManager->closeForm();
    }
}

//void BookingForm::renderCommon(sf::RenderWindow& window){}
//bool BookingForm::handleCommonInput(sf::Event event){}

void BookingForm::handleInput(sf::Event event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !fields[activeField]->getValueAsString().empty()) {
            fields[activeField]->getValueAsString();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            fields[activeField]->getValueAsString() += static_cast<char>(event.text.unicode);
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            activeField = (activeField + 1) % fields.size();
        }
        if (event.key.code == sf::Keyboard::Return) {
            std::cout << "Entered Data: ";
            for (const auto& field : fields) std::cout << field << " ";
            std::cout << std::endl;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        int yOffset = 60;  // ✅ Start from the top

        for (std::size_t i = 0; i < fields.size(); ++i) {
            sf::FloatRect inputBoxBounds(260, yOffset - 5, 250, 35);  // ✅ Uses dynamic positioning
            if (inputBoxBounds.contains(mousePos)) {
                activeField = i;
                return;
            }
            yOffset += 50;  // ✅ Move down dynamically (same logic as `render()`)
        }

        //לבינתיים
        //// ✅ Handle Room Type Button Click          
        //float roomButtonX = 10;
        //for (int i = 0; i < roomTypeSelection.size(); ++i) {
        //    sf::FloatRect timeButtonBounds(roomButtonX, yOffset, 150, 30);
        //    if (timeButtonBounds.contains(mousePos)) {
        //        selectedRoomType = i;
        //        //Update the "Room Type" input box
        //        userInput[8] = roomTypeSelection[i];

        //        return;
        //    }
        //    roomButtonX += 160;  // Move to next button
        //}

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

void BookingForm::renderCommon(sf::RenderWindow& window) {
	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");

    sf::Text title(getFormType(), font, 26);
    title.setFillColor(sf::Color(20, 20, 20));
    title.setStyle(sf::Text::Bold);
    title.setPosition(20, 10);
    window.draw(title);

    bool cursorVisible = (cursorTimer.getElapsedTime().asMilliseconds() % 1000 < 500);

	for (std::size_t i = 0; i < fields.size(); ++i) {
		fields[i]->render(window, font, 260, 60 + i * 50, i == activeField, showCursor);
	}

	for (auto& button : buttons) {
		button.render(window, font);
	}
}


