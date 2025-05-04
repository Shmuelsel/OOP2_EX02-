#ifndef HOTELBOOKINGFORM_H
#define HOTELBOOKINGFORM_H

#include "BookingForm.h"


class HotelBookingForm : public BookingForm {
public:
    HotelBookingForm(sf::RenderWindow& win, DialogueManager* manager);
    std::string getFormType() const override;
    void render(sf::RenderWindow& window) override;
    void handleInput(sf::Event event) ;
	std::string validateForm() const override;
private:
	int selectedRoomType = 0;
	void initializeFields() override;

};

#endif
