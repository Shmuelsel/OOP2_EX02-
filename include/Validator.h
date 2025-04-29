// Validator.h
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <memory>

template <typename T>
class Validator {
public:
    virtual ~Validator() = default;
    virtual bool validate(const T& value) const = 0;
    virtual std::string getErrorMessage() const = 0;
};

// ולידטור לשם (לא מכיל ספרות)
class NameValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
        return value.find_first_of("0123456789") == std::string::npos;
    }
    std::string getErrorMessage() const override {
        return "Name cannot contain digits.";
    }
};

// ולידטור לתעודת זהות (ספרת ביקורת)
class IDValidator : public Validator<std::string> {
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

// ולידטור למייל
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

// ולידטור לכתובת (Street-Number-City)
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

// ולידטור לתאריך (DD-MM-YYYY)
class DateValidator : public Validator<std::string> {
public:
    bool validate(const std::string& value) const override {
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

// ולידטור למספרים בטווח (למשל, מספר אורחים)
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

// ולידטור לבחירה מרובה (למשל, Preferred Time)
class MultiChoiceValidator : public Validator<std::vector<std::string>> {
private:
    std::vector<std::string> validOptions;

public:
    MultiChoiceValidator(const std::vector<std::string>& options) : validOptions(options) {}
    bool validate(const std::vector<std::string>& value) const override {
        if (value.size() == 1 && value[0] == "Don't Care") return true;
        for (const auto& val : value) {
            if (std::find(validOptions.begin(), validOptions.end(), val) == validOptions.end()) {
                return false;
            }
        }
        return !value.empty();
    }
    std::string getErrorMessage() const override {
        return "Invalid selection for multi-choice field.";
    }
};

#endif