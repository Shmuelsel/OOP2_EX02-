#ifndef FIELD_H
#define FIELD_H

#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Validator.h"
#include "Button.h"

class FieldBase {
public:
    virtual ~FieldBase() = default;
    virtual void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const = 0;
    virtual void handleInput(sf::Event event) = 0;
    virtual std::string getValueAsString() const = 0;
    virtual std::string getLabel() const = 0;
    virtual void setValueFromString(const std::string& str) = 0;
    virtual std::vector<std::string> validate() const = 0;
    virtual void handleClick(const sf::Vector2f& mousePos) = 0; // פונקציה חדשה לטיפול בלחיצות
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
        inputBox.setPosition(240, y - 5);
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(isActive ? sf::Color(0, 120, 255) : sf::Color(160, 160, 160));
        window.draw(inputBox);

        std::string displayText = getValueAsString();
        if (isActive && cursorVisible) displayText += "|";

        sf::Text inputText(displayText, font, 16);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(245, y);
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

    void handleClick(const sf::Vector2f& mousePos) override {
        // שדות רגילים (למשל, טקסט או מספרים) לא מטפלים בלחיצות
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

    void setValueFromString(const std::string& str) override {
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

    void setValue(const T& newValue) {
        value = newValue;
    }
};

// Specialization עבור שדה בחירה מרובה (כמו Room Type)
template <>
class Field<std::vector<std::string>> : public FieldBase {
private:
    std::string label;
    std::vector<std::string> value;
    std::vector<std::string> options;
    std::vector<Button> buttons;
    std::vector<std::unique_ptr<Validator<std::vector<std::string>>>> validators;
    float startX, startY, buttonWidth, buttonHeight;

public:

    Field(const std::string& label, const std::vector<std::string>& options,
        const std::vector<std::string>& defaultValue = {},
        float startX = 10, float startY = 520, float buttonWidth = 150, float buttonHeight = 30)
        : label(label), value(defaultValue), options(options),
        startX(startX), startY(startY), buttonWidth(buttonWidth), buttonHeight(buttonHeight) {
        float buttonX = startX;

        for (const auto& option : options) {
            buttons.emplace_back(option, buttonX, startY, buttonWidth, buttonHeight);
            buttonX += buttonWidth + 10;
        }
        for (size_t i = 0; i < options.size(); ++i) {
            buttons[i].setSelected(std::find(value.begin(), value.end(), options[i]) != value.end());
        }
    }

    void addValidator(std::unique_ptr<Validator<std::vector<std::string>>> validator) {
        validators.push_back(std::move(validator));
    }


    /*sf::Text labelText(label, font, 18);
    labelText.setFillColor(sf::Color(60, 60, 60));
    labelText.setPosition(x, y);
    window.draw(labelText);

    
}*/

    void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const override {
        sf::Text labelText(label, font, 18);
        labelText.setFillColor(sf::Color(60, 60, 60));
        labelText.setPosition(x, y);
        window.draw(labelText);

        sf::RectangleShape inputBox(sf::Vector2f(350, 35));
        inputBox.setPosition(240, y - 5);
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(isActive ? sf::Color(0, 120, 255) : sf::Color(160, 160, 160));
        window.draw(inputBox);

        std::string displayText = getValueAsString();
        if (isActive && cursorVisible) displayText += "|";

        sf::Text inputText(displayText, font, 16);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(245, y);
        window.draw(inputText);

        for (const auto& button : buttons) {
            button.render(window, font);
        }

        /*std::string valueText = getValueAsString();
        sf::Text valueDisplay(valueText, font, 16);
        valueDisplay.setFillColor(sf::Color::Black);
        valueDisplay.setPosition(startX, startY + buttonHeight + 10);
        window.draw(valueDisplay);*/
    }

    void handleInput(sf::Event event) override {
        // אין טיפול בקלט טקסטואלי עבור שדה בחירה מרובה

    }

    void handleClick(const sf::Vector2f& mousePos) override {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].handleClick(mousePos)) {
                if (options[i] == "Don't Care") {
                    value.clear();
                    for (auto& button : buttons) {
                        button.setSelected(false);
                    }
                    buttons[i].setSelected(true);
                    value = { "Don't Care" };
                }
                else {
                    for (size_t j = 0; j < options.size(); ++j) {
                        if (options[j] == "Don't Care") {
                            buttons[j].setSelected(false);
                        }
                    }
                    buttons[i].setSelected(!buttons[i].getSelected());
                    value.clear();
                    for (size_t j = 0; j < buttons.size(); ++j) {
                        if (buttons[j].getSelected()) {
                            value.push_back(options[j]);
                        }
                    }
                }
                return;
            }
        }
    }

    std::string getValueAsString() const override {
        std::string result;
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) result += "| ";
            result += value[i];
        }
        return result.empty() ? "None" : result;
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

    void setValue(const std::vector<std::string>& newValue) {
        value = newValue;
        for (auto& button : buttons) {
            button.setSelected(false);
        }
        for (const auto& val : newValue) {
            auto it = std::find(options.begin(), options.end(), val);
            if (it != options.end()) {
                buttons[std::distance(options.begin(), it)].setSelected(true);
            }
        }
    }

    void setValueFromString(const std::string& str) override {
        std::vector<std::string> newValue;
        if (str != "None") {
            std::string current;
            for (char c : str) {
                if (c == '|' && !current.empty()) {
                    newValue.push_back(current);
                    current.clear();
                }
                else {
                    current += c;
                }
            }
            if (!current.empty()) {
                newValue.push_back(current);
            }
        }
        setValue(newValue);
    }
};

#endif