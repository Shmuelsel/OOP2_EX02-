#ifndef BOOKINGFORM_H
#define BOOKINGFORM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <array>
#include "Field.h"
#include "Button.h"

class DialogueManager;

struct PersonalInfo {
    std::string name;
    std::string id;
    std::string address;
    std::string email;
};

class BookingForm {
protected:
	std::vector<std::string> fieldLabels;//need to define the labels for the fields
	std::vector<std::string> userInput;  //need to define the input fields
    PersonalInfo personalInfo;

    sf::RenderWindow& window;
    DialogueManager* formManager;
    std::vector<std::unique_ptr<FieldBase>> fields;
	
    std::size_t activeField = 0;
    sf::Clock cursorTimer;
	virtual void initializeFields();
    virtual std::string setDefaultDate();
    bool handleCommonInput(sf::Event event);
    bool showCursor = true;
	std::vector<Button> buttons;
	virtual std::string validateForm() const = 0;
	//void validateFields();

public:
    BookingForm(sf::RenderWindow& win, DialogueManager* manager);
    virtual ~BookingForm() = default;
    virtual void render(sf::RenderWindow& window) = 0;
    void handleInput(sf::Event event);
    virtual std::string getFormType() const = 0;
    void openConfirmationWindow();
    void renderCommon(sf::RenderWindow& window);
};

#endif // BOOKINGFORM_H
