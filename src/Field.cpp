#pragma once
#include "FieldBase.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <iostream>


template <typename T>
class Field : public FieldBase {
private:
    std::string label;
    T value;
    std::vector<std::unique_ptr<Validator>> validators;

public:
    Field(const std::string& label, const T& defaultValue = T())
        : label(label), value(defaultValue) {
    }

    void addValidator(std::unique_ptr<Validator> validator) {
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

        std::string displayText = getValue();
        if (isActive && cursorVisible) displayText += "|";

        sf::Text inputText(displayText, font, 16);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(x + 225, y);
        window.draw(inputText);
    }

    void handleInput(sf::Event event, std::string& value) override {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !value.empty()) {
                value.pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                value += static_cast<char>(event.text.unicode);
            }
            setValue(value);
        }
    }

    std::string getValue() const override {
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

    void setValue(const std::string& str) {
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

    std::string getLabel() const override {
        return label;
    }

    std::vector<std::string> validate() const override {
        std::vector<std::string> errors;
        for (const auto& validator : validators) {
            if (!validator->validate(getValue())) {
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
    std::vector<std::unique_ptr<Validator>> validators;

public:
    Field(const std::string& label, const std::vector<std::string>& options)
        : label(label), options(options), selected(options.size(), false) {
        value = { "Don't Care" };
        selected[options.size() - 1] = true; // ברירת מחדל: "Don't Care"
    }

    void addValidator(std::unique_ptr<Validator> validator) {
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

    void handleInput(sf::Event event, std::string& value) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            float buttonX = 240;
            for (size_t i = 0; i < options.size(); ++i) {
                sf::FloatRect bounds(buttonX, 60 + (fieldLabels.size() - 1) * 50, 100, 30);
                if (bounds.contains(mousePos)) {
                    if (options[i] == "Don't Care") {
                        selected = std::vector<bool>(options.size(), false);
                        selected[i] = true;
                        value = "Don't Care";
                    }
                    else {
                        selected[options.size() - 1] = false; // בטל "Don't Care"
                        selected[i] = !selected[i];
                        value = "";
                        for (size_t j = 0; j < options.size(); ++j) {
                            if (selected[j]) {
                                if (!value.empty()) value += "| ";
                                value += options[j];
                            }
                        }
                    }
                    setValue(value);
                    return;
                }
                buttonX += 110;
            }
        }
    }

    std::string getValue() const override {
        std::string result;
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) result += "| ";
            result += value[i];
        }
        return result;
    }

    void setValue(const std::string& str) {
        value.clear();
        std::string temp = str;
        size_t pos = 0;
        while ((pos = temp.find("| ")) != std::string::npos) {
            value.push_back(temp.substr(0, pos));
            temp.erase(0, pos + 2);
        }
        if (!temp.empty()) value.push_back(temp);

        selected = std::vector<bool>(options.size(), false);
        for (const auto& val : value) {
            for (size_t i = 0; i < options.size(); ++i) {
                if (val == options[i]) {
                    selected[i] = true;
                }
            }
        }
    }

    std::string getLabel() const override {
        return label;
    }

    std::vector<std::string> validate() const override {
        std::vector<std::string> errors;
        for (const auto& validator : validators) {
            if (!validator->validate(getValue())) {
                errors.push_back(label + ": " + validator->getErrorMessage());
            }
        }
        return errors;
    }
};