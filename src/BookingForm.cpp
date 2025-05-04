#include "BookingForm.h"
#include "DialogueManager.h"
#include <iostream>
#include <ctime>  
#include "config.h"
#include "Field.h"
#include "Button.h"
#include <iomanip>
#include <sstream>

BookingForm::BookingForm(sf::RenderWindow& win, DialogueManager* manager) :window(win), formManager(manager) {

	font = DialogueManager::getFont();

    title.setFont(font);
    title.setCharacterSize(26);
    title.setFillColor(sf::Color(20, 20, 20));
    title.setStyle(sf::Text::Bold);
    title.setPosition(20, 10);
	initializeFields();
}

std::string BookingForm::setDefaultDate() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    std::ostringstream oss;
    oss << (1900 + ltm.tm_year) << "-"
        << std::setw(2) << std::setfill('0') << (1 + ltm.tm_mon) << "-"
        << std::setw(2) << std::setfill('0') << ltm.tm_mday;

    return oss.str();
}

void BookingForm::initializeFields() {
	fields.push_back(std::make_unique<Field<std::string>>("Name:", "", std::make_unique<NameValidator>()));
	fields.push_back(std::make_unique<Field<int>>("ID:", 0, std::make_unique<IDValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Address:", "", std::make_unique<AddressValidator>()));
	fields.push_back(std::make_unique<Field<std::string>>("Email:", "", std::make_unique<EmailValidator>()));

    buttons.push_back(Button("DONE", 20, 570, 140, 40, sf::Color(50,150,50)));
    buttons.push_back(Button("CANCEL", 200, 570, 140, 40, sf::Color(180,0,0)));
}

void BookingForm::openConfirmationWindow() {
    sf::RenderWindow confirmWindow(sf::VideoMode(600, 650), "Confirm", sf::Style::Titlebar | sf::Style::Close);
   
    sf::Text title("Confirm", font, 24);
    title.setFillColor(sf::Color::Black);
    title.setPosition(250, 20);

    std::vector<sf::Text> fieldTexts;
    std::vector<sf::Text> errorTexts;
    float yPos = 70;
    bool hasError = false;
    // הצגת השדות והשגיאות
	chackForErrors(fieldTexts, errorTexts, hasError, yPos);

    // הוספת קו מפריד
    sf::Text separator("****************************", font, 16);
    separator.setFillColor(sf::Color::Black);
    separator.setPosition(50, yPos);
    yPos += 30;

    // ולידציה כללית של הטופס
    std::string formError = validateForm();
    sf::Text formValidatorTitle("Form validators:", font, 16);
    formValidatorTitle.setFillColor(sf::Color::Black);
    formValidatorTitle.setPosition(50, yPos);
    yPos += 20;

    sf::Text formErrorText(formError, font, 14);
    formErrorText.setFillColor(sf::Color::Red);
    formErrorText.setPosition(50, yPos);
    yPos += 30;

    Button approveButton("APPROVE", 150, yPos, 130, 40, sf::Color(50,150,50));
    Button cancelButton("CANCEL", 350, yPos, 130, 40, sf::Color(180,0,0));

    bool approved = false;
    while (confirmWindow.isOpen()) {
        sf::Event event;
        while (confirmWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                confirmWindow.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (approveButton.handleClick(mousePos)) {
                    if (hasError) {
                        confirmWindow.close();
                    }
                    else {
                    approved = true;
                }
                    confirmWindow.close();
                }
                else if (cancelButton.handleClick(mousePos)) {
                    approved = false;
                    confirmWindow.close();
                }
            }
        }

        confirmWindow.clear(sf::Color::White);
        confirmWindow.draw(title);
        for (const auto& text : fieldTexts) {
            confirmWindow.draw(text);
        }
        for (const auto& text : errorTexts) {
            confirmWindow.draw(text);
        }
        confirmWindow.draw(separator);
        confirmWindow.draw(formValidatorTitle);
        if (!formError.empty()) {
            confirmWindow.draw(formErrorText);
        }
        approveButton.render(confirmWindow, font);
        cancelButton.render(confirmWindow, font);
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
                    if (button.getText() == "DONE") {
                        openConfirmationWindow();
                        return;
                    }
                    else if (button.getText() == "CANCEL") {
                        std::cout << "Cancelled " << getFormType() << std::endl;
                        formManager->closeForm();
                        return;
                    }
                }
            }

            // טיפול בבחירת שדה פעיל
            float yOffset = 60;
            for (size_t i = 0; i < fields.size(); ++i) {
                sf::FloatRect inputBoxBounds(240, yOffset - 5, 350, 35);
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
	
    title.setString(getFormType());
    window.draw(title);

    bool cursorVisible = (cursorTimer.getElapsedTime().asMilliseconds() % 1000 < 500);

    for (std::size_t i = 0; i < fields.size(); ++i) {
        fields[i]->render(window, font, 20, 60 + i * 50, i == activeField, cursorVisible);
    }

    for (auto& button : buttons) {
        button.render(window, font);
    }
}

void BookingForm::chackForErrors(std::vector<sf::Text>& fieldTexts, std::vector<sf::Text>& errorTexts, bool& hasError, float& yPos) {
    for (size_t i = 0; i < fields.size(); ++i) {
        std::string fieldDisplay = fields[i]->getLabel() + " " + fields[i]->getValueAsString();
        sf::Text fieldText(fieldDisplay, font, 16);
        fieldText.setFillColor(sf::Color::Black);
        fieldText.setPosition(50, yPos);
        fieldTexts.push_back(fieldText);

        if (fields[i]->validate() != "") {
            std::string errorMsg = fields[i]->validate(); 
            sf::Text errorText(errorMsg, font, 14);
            errorText.setFillColor(sf::Color::Red);
            errorText.setPosition(50, yPos + 20);
            errorTexts.push_back(errorText);
            hasError = true;
        }

        yPos += hasError ? 50 : 30; 
    }
}