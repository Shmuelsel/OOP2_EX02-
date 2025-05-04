#ifndef FIELD_H
#define FIELD_H

#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Validator.h"
#include "Button.h"
#include "FieldBase.h"

class DialogueManager;

template <typename T>
class Field : public FieldBase {
private:
    std::string label;
    T value;
    std::unique_ptr<Validator<T>> validator;
    sf::Text labelText;
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Font font;

public:
    Field(const std::string& label, const T& defaultValue, std::unique_ptr<Validator<T>> validator)
        : label(label), value(defaultValue), validator(std::move(validator)) {

		font = DialogueManager::getFont();
		labelText.setFont(font);
		labelText.setCharacterSize(18);
		labelText.setFillColor(sf::Color(60, 60, 60));
		labelText.setString(label);

		inputBox.setSize(sf::Vector2f(350, 35));
		inputBox.setFillColor(sf::Color::White);    
		inputBox.setOutlineThickness(2);

		inputText.setFont(font);
		inputText.setCharacterSize(16);
		inputText.setFillColor(sf::Color::Black);
    }

    void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) override {
        labelText.setPosition(x, y);
        window.draw(labelText);

        inputBox.setPosition(240, y - 5);
        inputBox.setOutlineColor(isActive ? sf::Color(0, 120, 255) : sf::Color(160, 160, 160));
        window.draw(inputBox);

        std::string displayText = getValueAsString();
        if (isActive && cursorVisible) displayText += "|";

		inputText.setString(displayText);
        inputText.setPosition(245, y);
        window.draw(inputText);
    }

    void handleInput(sf::Event event) override {
        if (event.type == sf::Event::TextEntered) {
            T currentValue = this->getValue();
            if (event.text.unicode == '\b') {
                if constexpr (std::is_same_v<T, int>) {
                    currentValue = currentValue / 10;
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    currentValue.pop_back();
                }
            }
            else if(event.text.unicode >= 32 && event.text.unicode < 128) {
                if constexpr (std::is_same_v<T, int>) {
                    currentValue = currentValue * 10 + (event.text.unicode - '0');
                }
                else if constexpr(std::is_same_v<T, std::string>) {
                    currentValue += static_cast<char>(event.text.unicode);
                }
                
            }
            setValue(currentValue);
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

    T getValue() {
        return value;
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

	void setValue(const T newValue){
		value = newValue;
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
};

#endif