#pragma once
#include "ViewBase.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuView : public ViewBase {
public:
    MenuView(sf::Font& font, int width, int height);
    void handleEvent(const sf::Event& event) override;
    void update() override {}
    void draw(sf::RenderWindow& window) override;
    int getSelected() const;
    bool isEnterPressed() const { return enterPressed; }
    void resetEnter() { enterPressed = false; }
private:
    std::vector<std::string> menuItems;
    int menuIndex;
    sf::Font& font;
    int width, height;
    bool enterPressed = false;
};