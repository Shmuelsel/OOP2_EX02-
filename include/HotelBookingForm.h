#ifndef HOTELBOOKINGFORM_H
#define HOTELBOOKINGFORM_H

#include "BookingForm.h"
#include <ctime>

class HotelBookingForm : public BookingForm {
private:
    void initializeFields() override;
    std::vector<std::string> validateForm() const override;
public:
    HotelBookingForm(sf::RenderWindow& win, DialogueManager* manager, const sf::Font& font);
    void render(sf::RenderWindow& window) override;
    void handleInput(sf::Event event) override;
    std::string getFormType() const override;
};

#endif