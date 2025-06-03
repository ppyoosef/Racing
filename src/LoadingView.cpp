#include "LoadingView.hpp"

LoadingView::LoadingView(sf::Font& font, int width, int height) {
    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(48);
    loadingText.setFillColor(sf::Color::White);
    sf::FloatRect bounds = loadingText.getLocalBounds();
    loadingText.setOrigin(bounds.width / 2, bounds.height / 2);
    loadingText.setPosition(width / 2, height / 2);
}
void LoadingView::update() {}
void LoadingView::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(loadingText);
}