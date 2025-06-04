#pragma once
#include "ViewBase.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Car.hpp"
#include "Line.hpp"
#include "Speedometer.hpp"

using namespace sf;

class GameView : public ViewBase {
public:
    GameView(sf::Font& font, int width, int height, int carIndex, int mapIndex);
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void drawQuad(sf::RenderWindow& w, sf::Color c, int x1, int y1, int w1, int x2, int y2, int w2);
    void draw(sf::RenderWindow& window) override;
    bool isEscapePressed() const { return escapePressed; }
    void resetEscape() { escapePressed = false; }
private:
    sf::Font& font;
    int width, height;
    bool escapePressed = false;

    // Game state
    Car playerCar;
    Speedometer speedometer;
    std::vector<Line> lines;
    Text speedText;
    Texture t[50];
    Texture bg;
    Sprite object[50];
    Music engineSound;
    Sprite sBackground;

    int N;
    float playerX, camX;
    int pos, H, currentSpeed;
    float carAngle;
    int carIndex, mapIndex;

    // Add other game resources as needed (textures, sounds, etc.)
};