#ifndef FLIGHTBOOKINGFORM_H
#define FLIGHTBOOKINGFORM_H

#include "BookingForm.h"
#include <SFML/Graphics.hpp>
#include "Field.h"

class FlightBookingForm : public BookingForm {
    
public:
    FlightBookingForm(sf::RenderWindow& win, DialogueManager* manager);
	void initializeFields();
    std::string getFormType() const override;
    void render(sf::RenderWindow& window) override;
    //void handleInput(sf::Event event) override;
};

#endif
