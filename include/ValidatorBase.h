#ifndef VALIDATOR_BASE_H
#define VALIDATOR_BASE_H

#include <string>

template <typename T>
class Validator {
public:
    virtual ~Validator() = default;
    virtual bool validate(const T& value) const = 0;
    virtual std::string getErrorMessage() const = 0;
};

#endif // VALIDATOR_BASE_H