#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "DialogueManager.h"
#include "Button.h"

class UIManager {
private:
    sf::RenderWindow& window;
    DialogueManager& formManager;
    sf::Font font;
	std::vector<Button> buttons;
	void initializeButtons();

public:
    UIManager(sf::RenderWindow& win, DialogueManager& dManager);
    void handleEvent(const sf::Event& event);
    void render();
};

#endif
