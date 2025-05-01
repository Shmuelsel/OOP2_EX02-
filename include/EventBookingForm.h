#ifndef EVENTBOOKINGFORM_H
#define EVENTBOOKINGFORM_H

#include "BookingForm.h"

class EventBookingForm : public BookingForm {
public:
    EventBookingForm(sf::RenderWindow& win, DialogueManager* manager);
    std::string getFormType() const override;
    void render(sf::RenderWindow& window) override;
	//const std::vector<std::unique_ptr<FieldBase>>& getFields() const override;
    //void handleInput(sf::Event event) override;
private:
    std::string setDefaultValues() override;
    std::array<std::string, 5> SeatingTypeSelection = { "General Admission", "Front Row", "VIP Section", "Aisle Seat" };
    int selectedSeatingType = 0;
};

#endif // EVENTBOOKINGFORM_H
