#ifndef MULTI_CHOICE_FIELD_H
#define MULTI_CHOICE_FIELD_H

#include "FieldBase.h"
#include "Validator.h"
#include "Button.h"
#include <vector>
#include <string>

template <>
class Field<std::vector<std::string>> : public FieldBase {
private:
    std::string label;
    std::vector<std::string> value;
    std::vector<std::string> options;
    std::vector<Button> buttons;
    std::unique_ptr<Validator<std::vector<std::string>>> validator;
    float startX, startY, buttonWidth, buttonHeight;
    sf::Text labelText;
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Font font;

public:
    Field(const std::string& label, const std::vector<std::string>& options,
        const std::vector<std::string>& defaultValue, std::unique_ptr<Validator<std::vector<std::string>>> validator,
        float startX = 10, float startY = 520, float buttonWidth = 150, float buttonHeight = 30)
        : label(label), value(defaultValue), options(options),
        startX(startX), startY(startY), buttonWidth(buttonWidth), buttonHeight(buttonHeight), validator(std::move(validator)) {
        float buttonX = startX;
        for (const auto& option : options) {
            buttons.emplace_back(option, buttonX, startY, buttonWidth, buttonHeight);
            buttonX += buttonWidth + 10;
        }
        for (size_t i = 0; i < options.size(); ++i) {
            buttons[i].setSelected(std::find(value.begin(), value.end(), options[i]) != value.end());
        }
    }

    void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible)  override {
        labelText.setFont(font);
        labelText.setCharacterSize(18);
        labelText.setString(label);
        labelText.setFillColor(sf::Color(60, 60, 60));
        labelText.setPosition(x, y);
        window.draw(labelText);

        inputBox.setSize(sf::Vector2f(350, 35));
        inputBox.setPosition(240, y - 5);
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(isActive ? sf::Color(0, 120, 255) : sf::Color(160, 160, 160));
        window.draw(inputBox);

        std::string displayText = getValueAsString();
        if (isActive && cursorVisible) displayText += "|";

        inputText.setFont(font);
        inputText.setCharacterSize(16);
        inputText.setString(displayText);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(245, y);
        window.draw(inputText);

        for (const auto& button : buttons) {
            button.render(window, font);
        }
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

    std::string validate() const override {
        if (validator && !validator->validate(value)) {
            return label + ": " + validator->getErrorMessage();
        }
        return "";
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

#endif // MULTI_CHOICE_FIELD_H