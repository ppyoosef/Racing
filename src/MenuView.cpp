#include "MenuView.hpp"

MenuView::MenuView(sf::Font& font, int width, int height)
    : font(font), width(width), height(height), menuIndex(0) {
    menuItems = { "Start Game", "Settings", "Quit" };
}

void MenuView::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
            menuIndex = (menuIndex + menuItems.size() - 1) % menuItems.size();
        if (event.key.code == sf::Keyboard::Down)
            menuIndex = (menuIndex + 1) % menuItems.size();
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
            enterPressed = true;
    }
}

void MenuView::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 30));
    sf::Text title("Racing Game", font, 60);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2, bounds.height / 2);
    title.setPosition(width / 2, 120);
    window.draw(title);

    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text item(menuItems[i], font, 40);
        item.setFillColor(i == menuIndex ? sf::Color::Red : sf::Color::White);
        sf::FloatRect ibounds = item.getLocalBounds();
        item.setOrigin(ibounds.width / 2, ibounds.height / 2);
        item.setPosition(width / 2, 250 + i * 70);
        window.draw(item);
    }
}

int MenuView::getSelected() const { return menuIndex; }