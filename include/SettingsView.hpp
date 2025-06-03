#pragma once
#include "ViewBase.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SettingsView : public ViewBase {
public:
    SettingsView(sf::Font& font, int width, int height,
                 const std::vector<std::string>& carOptions,
                 const std::vector<std::string>& mapOptions);

    void handleEvent(const sf::Event& event) override;
    void update() override {}
    void draw(sf::RenderWindow& window) override;

    int getCarIndex() const { return carIndex; }
    int getMapIndex() const { return mapIndex; }
    bool isBackSelected() const { return backSelected; }
    void resetBack() { backSelected = false; }

private:
    sf::Font& font;
    int width, height;
    std::vector<std::string> carOptions, mapOptions;
    int carIndex = 0, mapIndex = 0, settingsIndex = 0;
    bool backSelected = false;
};