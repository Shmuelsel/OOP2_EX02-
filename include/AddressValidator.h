#ifndef ADDRESS_VALIDATOR_H
#define ADDRESS_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

class AddressValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
        size_t firstDash = value.find('-');
        size_t secondDash = value.find('-', firstDash + 1);
        if (firstDash == std::string::npos || secondDash == std::string::npos) return false;

        std::string street = value.substr(0, firstDash);
        std::string number = value.substr(firstDash + 1, secondDash - firstDash - 1);
        std::string city = value.substr(secondDash + 1);

        if (street.empty() || number.empty() || city.empty()) return false;
        if (street.find_first_of("0123456789") != std::string::npos) return false;
        if (city.find_first_of("0123456789") != std::string::npos) return false;
        for (char c : number) {
            if (!std::isdigit(c)) return false;
        }
        return true;
    }
    std::string getErrorMessage() const override {
        return "Invalid address format (Street-Number-City).";
    }
};

#endif // ADDRESS_VALIDATOR_H