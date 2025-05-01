#include "BookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  // For getting current date
#include "config.h"
#include "Field.h"
#include "Button.h"

BookingForm::BookingForm(sf::RenderWindow& win, DialogueManager* manager) :window(win), formManager(manager) {	
	initializeFields();
}

std::string BookingForm::setDefaultDate() {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);  // Safe alternative to localtime()
	return std::to_string(1900 + ltm.tm_year) + "-" +
		std::to_string(1 + ltm.tm_mon) + "-" +
		std::to_string(ltm.tm_mday);
}

void BookingForm::initializeFields() {
	fields.push_back(std::make_unique<Field<std::string>>("Name:"));
	fields.push_back(std::make_unique<Field<std::string>>("ID:"));
	fields.push_back(std::make_unique<Field<std::string>>("Address:"));
	fields.push_back(std::make_unique<Field<std::string>>("Email:"));

	buttons.push_back(Button("Done", 20, 570, 140, 40, sf::Color::Green));
	buttons.push_back(Button("Cancel", 200, 570, 140, 40, sf::Color::Red));
}

void BookingForm::openConfirmationWindow() {
    const std::string& formTitle = getFormType();
    sf::RenderWindow confirmWindow(sf::VideoMode(500, 600), "Confirm " + formTitle);

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arialbd.ttf");

    bool approved = false;

    std::unique_ptr<Button> approveButton = std::make_unique<Button>("Approve", 100, 300, 120, 40, sf::Color::Green);
    std::unique_ptr<Button> cancelButton = std::make_unique<Button>("Cancel", 280, 300, 120, 40, sf::Color::Red);


    while (confirmWindow.isOpen()) {
        sf::Event event;

        while (confirmWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                confirmWindow.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (approveButton->handleClick(mousePos)) {
                    std::cout << formTitle << " Confirmed! Returning to main menu." << std::endl;
                    approved = true;
                    confirmWindow.close();

                }
                else if (cancelButton->handleClick(mousePos)) {
                    std::cout << formTitle << " Cancelled! Returning to main menu." << std::endl;
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

		
		approveButton->render(confirmWindow, font);
		cancelButton->render(confirmWindow, font);
        confirmWindow.display();
    }

    if (approved) {
        formManager->closeForm();
    }
}

void BookingForm::handleInput(sf::Event event) {
    try {
        if (activeField < fields.size()) {
            fields[activeField]->handleInput(event);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
            activeField = (activeField + 1) % fields.size();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            std::cout << "Current Form Data (" << getFormType() << "):\n";
            for (const auto& field : fields) {
                std::cout << field->getLabel() << " " << field->getValueAsString() << "\n";
            }
            std::cout << "------------------------\n";
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            // טיפול בלחיצות על כפתורי בחירה מרובה של השדות
            for (size_t i = 0; i < fields.size(); ++i) {
                fields[i]->handleClick(mousePos);
            }

            // טיפול בלחיצות על כפתורי Done ו-Cancel
            for (auto& button : buttons) {
                if (button.handleClick(mousePos)) {
                    if (button.getText() == "Done") {
                        openConfirmationWindow();
                        return;
                    }
                    else if (button.getText() == "Cancel") {
                        std::cout << "Cancelled " << getFormType() << std::endl;
                        formManager->closeForm();
                        return;
                    }
                }
            }

            // טיפול בבחירת שדה פעיל
            float yOffset = 60;
            for (size_t i = 0; i < fields.size(); ++i) {
                sf::FloatRect inputBoxBounds(240, yOffset - 5, 250, 35);
                if (inputBoxBounds.contains(mousePos)) {
                    activeField = i;
                    return;
                }
                yOffset += 50;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in BookingForm::handleInput: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception in BookingForm::handleInput" << std::endl;
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


