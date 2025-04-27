#pragma once
#include <vector>

class FieldBase {
public:
    virtual ~FieldBase() = default;
    virtual void render(sf::RenderWindow& window, const sf::Font& font, float x, float y, bool isActive, bool cursorVisible) const = 0;
    virtual void handleInput(sf::Event event, std::string& value) = 0;
    virtual std::string getValue() const = 0;
    virtual std::string getLabel() const = 0;
    virtual std::vector<std::string> validate() const = 0;
};