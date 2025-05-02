// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
private:
    std::string text;
    sf::Vector2f position;
    sf::Vector2f size;
	bool isSelected;
    bool useFixedColor;
	sf::Color fixedColor;
    sf::Color selectedColor;
    sf::Color unselectedColor;
    sf::Color textSelectedColor;
    sf::Color textUnselectedColor;

public:
    Button(const std::string& text, float x, float y, float width = 100, float height = 30)
		: text(text), position(x, y), size(width, height), isSelected(false),useFixedColor(false),
        selectedColor(sf::Color(0, 120, 255)), unselectedColor(sf::Color::White),
        textSelectedColor(sf::Color::White), textUnselectedColor(sf::Color::Black) {
    }

    Button(const std::string& text, float x, float y, float width, float height, sf::Color fixedColor)
        : text(text), position(x, y), size(width, height), isSelected(false), useFixedColor(true),
        fixedColor(fixedColor), selectedColor(sf::Color::Transparent),
        unselectedColor(sf::Color::Transparent), textSelectedColor(sf::Color::White),
        textUnselectedColor(sf::Color::White) {
    }

    void render(sf::RenderWindow& window, const sf::Font& font) const {
        sf::RectangleShape buttonShape(size);
        buttonShape.setPosition(position);
		sf::Text buttonText;
        
        if (useFixedColor) {
            buttonShape.setFillColor(fixedColor);
            buttonText.setFillColor(sf::Color::White);
            buttonText.setString(text);
            buttonText.setFont(font);
            buttonText.setCharacterSize(24);
        }
        else {
            buttonShape.setFillColor(sf::Color::White);
            buttonText.setFillColor(isSelected ? textSelectedColor : textUnselectedColor);
			buttonText.setString(text);
			buttonText.setFont(font);
			buttonText.setCharacterSize(16);
			buttonShape.setOutlineThickness(2);
			buttonShape.setOutlineColor(sf::Color(160,160,160));
			buttonShape.setFillColor(isSelected ? selectedColor : unselectedColor);
        }   
        buttonText.setOrigin(buttonText.getLocalBounds().width / 2, buttonText.getLocalBounds().height / 2);
        buttonText.setPosition(position.x + size.x / 2, position.y + size.y / 2);
       
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    bool handleClick(const sf::Vector2f& mousePos) {
        sf::FloatRect bounds(position.x, position.y, size.x, size.y);
        if (bounds.contains(mousePos)) {
            if (useFixedColor) {
                isSelected = !isSelected;
            }
            return true;
        }
        return false;
    }

    void setSelected(bool selected) {
        isSelected = selected;
    }

    bool getSelected() const {
        return isSelected;
    }

    const std::string& getText() const {
        return text;
    }
};

#endif