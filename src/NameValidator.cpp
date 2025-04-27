#pragma once
#include "Validator.h"
class NameValidator : public Validator {
public:
    bool validate(const std::string& value) const override {
        return value.find_first_of("0123456789") == std::string::npos;
    }
    std::string getErrorMessage() const override {
        return "Name cannot contain digits.";
    }
};