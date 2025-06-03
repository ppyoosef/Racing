#include "SettingsView.hpp"

SettingsView::SettingsView(sf::Font& font, int width, int height,
                           const std::vector<std::string>& carOptions,
                           const std::vector<std::string>& mapOptions)
    : font(font), width(width), height(height),
      carOptions(carOptions), mapOptions(mapOptions) {}

void SettingsView::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
            settingsIndex = (settingsIndex + 2) % 3;
        if (event.key.code == sf::Keyboard::Down)
            settingsIndex = (settingsIndex + 1) % 3;
        if (event.key.code == sf::Keyboard::Left) {
            if (settingsIndex == 0)
                carIndex = (carIndex + carOptions.size() - 1) % carOptions.size();
            if (settingsIndex == 1)
                mapIndex = (mapIndex + mapOptions.size() - 1) % mapOptions.size();
        }
        if (event.key.code == sf::Keyboard::Right) {
            if (settingsIndex == 0)
                carIndex = (carIndex + 1) % carOptions.size();
            if (settingsIndex == 1)
                mapIndex = (mapIndex + 1) % mapOptions.size();
        }
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
            if (settingsIndex == 2)
                backSelected = true;
        }
    }
}

void SettingsView::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(20, 20, 40));
    sf::Text title("Settings", font, 54);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2, bounds.height / 2);
    title.setPosition(width / 2, 100);
    window.draw(title);

    // Car selection
    sf::Text carLabel("Car:", font, 36);
    carLabel.setPosition(width / 2 - 200, 220);
    window.draw(carLabel);
    sf::Text carChoice(carOptions[carIndex], font, 36);
    carChoice.setFillColor(settingsIndex == 0 ? sf::Color::Red : sf::Color::White);
    carChoice.setPosition(width / 2, 220);
    window.draw(carChoice);

    // Map selection
    sf::Text mapLabel("Map:", font, 36);
    mapLabel.setPosition(width / 2 - 200, 300);
    window.draw(mapLabel);
    sf::Text mapChoice(mapOptions[mapIndex], font, 36);
    mapChoice.setFillColor(settingsIndex == 1 ? sf::Color::Red : sf::Color::White);
    mapChoice.setPosition(width / 2, 300);
    window.draw(mapChoice);

    // Back option
    sf::Text back("Back", font, 36);
    back.setFillColor(settingsIndex == 2 ? sf::Color::Red : sf::Color::White);
    back.setPosition(width / 2 - 50, 400);
    window.draw(back);
}