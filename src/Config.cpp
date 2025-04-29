#include "Config.h"

namespace Config {

    std::vector<std::string> fieldLabelsBase = { "Name:", "ID:", "Address:", "Email:" };
    std::vector<std::string> fieldLabelsHotel = { "Hotel Name:", "Check-in Date:", "Check-out Date:", "Number of Guests:", "Room Type:" };
    std::vector<std::string> fieldLabelsFlight = { "Departure Airport:", "Arrival Airport:", "Departure Date:", "Preferred Time:" };

	std::vector<std::unique_ptr<FieldBase>> fieldsForm = {
		std::make_unique<Field<std::string>>("Name:"),
		std::make_unique<Field<std::string>>("ID:"),
		std::make_unique<Field<std::string>>("Address:"),
		std::make_unique<Field<std::string>>("Email:")
	};
      
    std::vector<std::unique_ptr<FieldBase>> fieldsHotel = {
        std::make_unique<Field<std::string>>("Hotel Name:"),
        std::make_unique<Field<std::string>>("Check-in Date:"),
        std::make_unique<Field<std::string>>("Check-out Date:"),
        std::make_unique<Field<int>>("Number of Guests:"),
        std::make_unique<Field<std::string>>("Room Type:")
    };

    std::vector<std::unique_ptr<FieldBase>> fieldsFlight = {
        std::make_unique<Field<std::string>>("Departure Airport:"),
        std::make_unique<Field<std::string>>("Arrival Airport:"),
        std::make_unique<Field<std::string>>("Departure Date:"),
        std::make_unique<Field<std::vector<std::string>>>("Preferred Time:", std::vector<std::string>{"Morning", "Afternoon", "Evening", "Don't Care"})
    };

}
