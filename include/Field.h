// Field.h
#ifndef FIELD_H
#define FIELD_H

#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Validator.h"

class FieldBase {
public:
    virtual ~FieldBase() = default;
    virtual void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const = 0;
    virtual void handleInput(sf::Event event) = 0;
    virtual std::string getValueAsString() const = 0;
    virtual std::string getLabel() const = 0;
    virtual std::vector<std::string> validate() const = 0;
};

template <typename T>
class Field : public FieldBase {
private:
    std::string label;
    T value;
    std::vector<std::unique_ptr<Validator<T>>> validators;

public:
    Field(const std::string& label, const T& defaultValue = T())
        : label(label), value(defaultValue) {
    }

    void addValidator(std::unique_ptr<Validator<T>> validator) {
        validators.push_back(std::move(validator));
    }

    void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const override {
        sf::Text labelText(label, font, 18);
        labelText.setFillColor(sf::Color(60, 60, 60));
        labelText.setPosition(x, y);
        window.draw(labelText);

        sf::RectangleShape inputBox(sf::Vector2f(350, 35));
        inputBox.setPosition(x + 220, y - 5);
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(isActive ? sf::Color(0, 120, 255) : sf::Color(160, 160, 160));
        window.draw(inputBox);

        std::string displayText = getValueAsString();
        if (isActive && cursorVisible) displayText += "|";

        sf::Text inputText(displayText, font, 16);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(x + 225, y);
        window.draw(inputText);
    }

    void handleInput(sf::Event event) override {
        if (event.type == sf::Event::TextEntered) {
            std::string currentValue = getValueAsString();
            if (event.text.unicode == '\b' && !currentValue.empty()) {
                currentValue.pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                currentValue += static_cast<char>(event.text.unicode);
            }
            setValueFromString(currentValue);
        }
    }

    std::string getValueAsString() const override {
        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        }
        else if constexpr (std::is_same_v<T, int>) {
            return std::to_string(value);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return value ? "Yes" : "No";
        }
        return "";
    }

    void setValueFromString(const std::string& str) {
        if constexpr (std::is_same_v<T, std::string>) {
            value = str;
        }
        else if constexpr (std::is_same_v<T, int>) {
            try {
                value = std::stoi(str);
            }
            catch (...) {}
        }
        else if constexpr (std::is_same_v<T, bool>) {
            value = (str == "Yes" || str == "yes");
        }
    }

    const T& getValue() const {
        return value;
    }

    std::string getLabel() const override {
        return label;
    }

    std::vector<std::string> validate() const override {
        std::vector<std::string> errors;
        for (const auto& validator : validators) {
            if (!validator->validate(value)) {
                errors.push_back(label + ": " + validator->getErrorMessage());
            }
        }
        return errors;
    }
};

// Specialization עבור שדה בחירה מרובה (כמו Preferred Time)
template <>
class Field<std::vector<std::string>> : public FieldBase {
private:
    std::string label;
    std::vector<std::string> value;
    std::vector<std::string> options;
    std::vector<bool> selected;
    std::vector<std::unique_ptr<Validator<std::vector<std::string>>>> validators;

public:
    Field(const std::string& label, const std::vector<std::string>& options)
        : label(label), options(options), selected(options.size(), false) {
        value = { "Don't Care" };
        selected[options.size() - 1] = true; // ברירת מחדל: "Don't Care"
    }

    void addValidator(std::unique_ptr<Validator<std::vector<std::string>>> validator) {
        validators.push_back(std::move(validator));
    }

    void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const override {
        sf::Text labelText(label, font, 18);
        labelText.setFillColor(sf::Color(60, 60, 60));
        labelText.setPosition(x, y);
        window.draw(labelText);

        float buttonX = x + 220;
        for (size_t i = 0; i < options.size(); ++i) {
            sf::RectangleShape button(sf::Vector2f(100, 30));
            button.setPosition(buttonX, y);
            button.setFillColor(selected[i] ? sf::Color(0, 120, 255) : sf::Color::White);
            button.setOutlineThickness(2);
            button.setOutlineColor(sf::Color(160, 160, 160));
            window.draw(button);

            sf::Text optionText(options[i], font, 16);
            optionText.setFillColor(selected[i] ? sf::Color::White : sf::Color::Black);
            optionText.setPosition(buttonX + 10, y + 5);
            window.draw(optionText);
            buttonX += 110;
        }
    }

    void handleInput(sf::Event event) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            float buttonX = 240;
            for (size_t i = 0; i < options.size(); ++i) {
                sf::FloatRect bounds(buttonX, 60 + (options.size() - 1) * 50, 100, 30);
                if (bounds.contains(mousePos)) {
                    if (options[i] == "Don't Care") {
                        selected = std::vector<bool>(options.size(), false);
                        selected[i] = true;
                        value = { "Don't Care" };
                    }
                    else {
                        selected[options.size() - 1] = false; // בטל "Don't Care"
                        selected[i] = !selected[i];
                        value.clear();
                        for (size_t j = 0; j < options.size(); ++j) {
                            if (selected[j]) {
                                value.push_back(options[j]);
                            }
                        }
                    }
                    return;
                }
                buttonX += 110;
            }
        }
    }

    std::string getValueAsString() const override {
        std::string result;
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) result += "| ";
            result += value[i];
        }
        return result;
    }

    const std::vector<std::string>& getValue() const {
        return value;
    }

    std::string getLabel() const override {
        return label;
    }

    std::vector<std::string> validate() const override {
        std::vector<std::string> errors;
        for (const auto& validator : validators) {
            if (!validator->validate(value)) {
                errors.push_back(label + ": " + validator->getErrorMessage());
            }
        }
        return errors;
    }
};

#endif