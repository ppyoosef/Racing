#pragma once
#include "ViewBase.hpp"
#include <SFML/Graphics.hpp>

class GameView : public ViewBase {
public:
    GameView(sf::Font& font, int width, int height);
    void handleEvent(const sf::Event& event) override;
    void update() override {}
    void draw(sf::RenderWindow& window) override;
    bool isEscapePressed() const { return escapePressed; }
    void resetEscape() { escapePressed = false; }
private:
    sf::Font& font;
    int width, height;
    bool escapePressed = false;
};