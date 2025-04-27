#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <functional>

class Validator {
public:
    virtual ~Validator() = default;
    virtual bool validate(const std::string& value) const = 0;
    virtual std::string getErrorMessage() const = 0;
};

#endif