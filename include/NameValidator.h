#ifndef NAME_VALIDATOR_H
#define NAME_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

class NameValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
        return value.find_first_of("0123456789") == std::string::npos;
    }
    std::string getErrorMessage() const override {
        return "Name cannot contain digits.";
    }
};

#endif // NAME_VALIDATOR_H