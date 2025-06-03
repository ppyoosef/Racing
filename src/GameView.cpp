#include "GameView.hpp"

GameView::GameView(sf::Font& font, int width, int height)
    : font(font), width(width), height(height) {}

void GameView::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape)
            escapePressed = true;
    }
}

void GameView::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    sf::Text msg("Game Running!\nPress ESC to return to menu.", font, 36);
    sf::FloatRect bounds = msg.getLocalBounds();
    msg.setOrigin(bounds.width / 2, bounds.height / 2);
    msg.setPosition(width / 2, height / 2);
    window.draw(msg);
}