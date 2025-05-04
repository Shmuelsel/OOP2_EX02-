#ifndef FIELD_BASE_H
#define FIELD_BASE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class FieldBase {
public:
    virtual ~FieldBase() = default;
    virtual void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) = 0;
    virtual void handleInput(sf::Event event) = 0;
    virtual std::string getValueAsString() const = 0;
    virtual std::string getLabel() const = 0;
    virtual std::string validate() const = 0;
    virtual void setValueFromString(const std::string& str) = 0;
    virtual void handleClick(const sf::Vector2f& mousePos) = 0;
};

#endif // FIELD_BASE_H