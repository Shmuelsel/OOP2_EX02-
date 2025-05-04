#ifndef RANGE_VALIDATOR_H
#define RANGE_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

template <typename T>
class RangeValidator : public Validator<T> {
private:
    T min;
    T max;

public:
    RangeValidator(T min, T max) : min(min), max(max) {}
    bool validate(const T& value) const override {
        return value >= min && value <= max;
    }
    std::string getErrorMessage() const override {
        return "Value must be between " + std::to_string(min) + " and " + std::to_string(max) + ".";
    }
};

#endif // RANGE_VALIDATOR_H