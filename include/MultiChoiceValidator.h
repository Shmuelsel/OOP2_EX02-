#ifndef MULTI_CHOICE_VALIDATOR_H
#define MULTI_CHOICE_VALIDATOR_H

#include "ValidatorBase.h"
#include <vector>
#include <string>
#include <algorithm>

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

#endif // MULTI_CHOICE_VALIDATOR_H