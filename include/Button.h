// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    std::string text;
    sf::Vector2f position;
    sf::Vector2f size;
    bool isSelected;
    sf::Color selectedColor;
    sf::Color unselectedColor;
    sf::Color textSelectedColor;
    sf::Color textUnselectedColor;

public:
    Button(const std::string& text, float x, float y, float width = 100, float height = 30)
        : text(text), position(x, y), size(width, height), isSelected(false),
        selectedColor(sf::Color(0, 120, 255)), unselectedColor(sf::Color::White),
        textSelectedColor(sf::Color::White), textUnselectedColor(sf::Color::Black) {
    }

    void render(sf::RenderWindow& window, const sf::Font& font) const {
        sf::RectangleShape buttonShape(size);
        buttonShape.setPosition(position);
        buttonShape.setFillColor(isSelected ? selectedColor : unselectedColor);
        buttonShape.setOutlineThickness(2);
        buttonShape.setOutlineColor(sf::Color(160, 160, 160));
        window.draw(buttonShape);

        sf::Text buttonText(text, font, 16);
        buttonText.setFillColor(isSelected ? textSelectedColor : textUnselectedColor);
        buttonText.setPosition(position.x + 10, position.y + 5);
        window.draw(buttonText);
    }

    bool handleClick(const sf::Vector2f& mousePos) {
        sf::FloatRect bounds(position.x, position.y, size.x, size.y);
        if (bounds.contains(mousePos)) {
            isSelected = !isSelected;
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