#ifndef DATE_VALIDATOR_H
#define DATE_VALIDATOR_H

#include "ValidatorBase.h"
#include <string>

class DateValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
        if (value.size() != 10) return false;
        if (value[4] != '-' || value[7] != '-') return false;

        std::string yearStr = value.substr(0, 4);
        std::string monthStr = value.substr(5, 2);
        std::string dayStr = value.substr(8, 2);

        try {
            int year = std::stoi(yearStr);
            int month = std::stoi(monthStr);
            int day = std::stoi(dayStr);
            return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 1900 && year <= 9999;
        }
        catch (...) {
            return false;
        }
    }

    std::string getErrorMessage() const override {
        return "Invalid date format (YYYY-MM-DD).";
    }

    /*bool validate(const std::string& value) const override {
        std::istringstream ss(value);
        std::string yearStr, monthStr, dayStr;

        if (!std::getline(ss, yearStr, '-')) return false;
        if (!std::getline(ss, monthStr, '-')) return false;
        if (!std::getline(ss, dayStr)) return false;

        try {
            int year = std::stoi(yearStr);
            int month = std::stoi(monthStr);
            int day = std::stoi(dayStr);

            return year >= 1900 && year <= 9999 &&
                month >= 1 && month <= 12 &&
                day >= 1 && day <= 31;
        }
        catch (...) {
            return false;
        }
    }*/
};

#endif // DATE_VALIDATOR_H