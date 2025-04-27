#pragma once
#include "Validator.h"

class IdValidator : public Validator {
public:
    bool validate(const std::string& value) const override {
        if (value.size() != 9) return false;
        int sum = 0;
        for (size_t i = 0; i < 9; ++i) {
            int digit = value[i] - '0';
            if (i % 2 == 1) digit *= 2;
            if (digit > 9) digit = (digit / 10) + (digit % 10);
            sum += digit;
        }
        return sum % 10 == 0;
    }
    std::string getErrorMessage() const override {
        return "Invalid ID (check digit).";
    }
};