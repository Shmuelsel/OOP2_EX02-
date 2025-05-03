// CarRentalForm.h - Handles Car Rental Bookings
#ifndef CARRENTALFORM_H
#define CARRENTALFORM_H

#include "BookingForm.h"
#include <array>
class CarRentalForm : public BookingForm {
public:
    CarRentalForm(sf::RenderWindow& win, DialogueManager* manager);    
    std::string getFormType() const override;
    void render(sf::RenderWindow& window) override;
	
private:
	void initializeFields() override;
	std::string validateForm() const override;
};

#endif // CARRENTALFORM_H
