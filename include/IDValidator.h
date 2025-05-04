#ifndef ID_VALIDATOR_H
#define ID_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

class IDValidator : public Validator<int> {
public:
    bool validate(const int& value) const override {
        int num = value;
        int digits[9];
        for (int i = 8; i >= 0; --i) {
            digits[i] = num % 10;
            num /= 10;
        }

        // Check if number had exactly 9 digits
        if (num > 0) return false;

        int sum = 0;
        for (int i = 0; i < 9; ++i) {
            int digit = digits[i];
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

#endif // ID_VALIDATOR_H