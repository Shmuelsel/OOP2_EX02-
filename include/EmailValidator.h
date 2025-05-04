#ifndef EMAIL_VALIDATOR_H
#define EMAIL_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

class EmailValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
        size_t atPos = value.find('@');
        if (atPos == std::string::npos || atPos == 0 || atPos == value.size() - 1) return false;
        size_t dotPos = value.find('.', atPos);
        if (dotPos == std::string::npos || dotPos == value.size() - 1) return false;
        if (value.find(' ', atPos) != std::string::npos) return false;
        return value.substr(dotPos + 1).size() >= 2;
    }
    std::string getErrorMessage() const override {
        return "Invalid email format.";
    }
};

#endif // EMAIL_VALIDATOR_H