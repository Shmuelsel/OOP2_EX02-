#pragma once
#include "Validator.h"

class DateValidator : public Validator {
public:
    bool validate(const std::string& value) const override {
        // фешоп: DD-MM-YYYY
        if (value.size() != 10) return false;
        if (value[2] != '-' || value[5] != '-') return false;

        std::string dayStr = value.substr(0, 2);
        std::string monthStr = value.substr(3, 2);
        std::string yearStr = value.substr(6, 4);

        try {
            int day = std::stoi(dayStr);
            int month = std::stoi(monthStr);
            int year = std::stoi(yearStr);
            return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 1900 && year <= 9999;
        }
        catch (...) {
            return false;
        }
    }
    std::string getErrorMessage() const override {
        return "Invalid date format (DD-MM-YYYY).";
    }
};