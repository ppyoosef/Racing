#pragma once
#include "ViewBase.hpp"
#include <SFML/Graphics.hpp>

class LoadingView : public ViewBase {
public:
    LoadingView(sf::Font& font, int width, int height);
    void handleEvent(const sf::Event& event) override {}
    void update() override;
    void draw(sf::RenderWindow& window) override;
private:
    sf::Text loadingText;
};