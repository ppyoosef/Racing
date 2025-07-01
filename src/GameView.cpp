#include "GameView.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>

#include "Speedometer.hpp"
#include "Car.hpp"
#include "Line.hpp"
#include "LoadingView.hpp"
#include "MenuView.hpp"
#include "SettingsView.hpp"
#include "GameView.hpp"

using namespace sf;
using namespace std;

// Car texture selection
std::vector<std::string> carFiles = {
    "images/car_red.png",
    "images/car_blue.png",
    "images/car_white.png"
};

GameView::GameView(sf::Font &font, int width, int height, int carIndex, int mapIndex)
    : font(font), width(width), height(height),
      playerCar(carFiles[carIndex], width / 2, height * 0.8f),
      speedometer(width - 130, 130, font),
      carIndex(carIndex), mapIndex(mapIndex)
{
    for (int i = 1; i <= 7; i++)
    {
        t[i].loadFromFile("images/" + std::to_string(i) + ".png");
        t[i].setSmooth(true);
        object[i].setTexture(t[i]);
    }

    bg.loadFromFile("images/bg.png");
    bg.setRepeated(true);

    sBackground.setTexture(bg);
    sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
    sBackground.setPosition(-2000, 0);

    // --- Load and play engine sound ---
    if (!engineSound.openFromFile("sounds/engine.ogg"))
    {
        std::cout << "Failed to load engine.ogg\n";
    }
    else
    {
        engineSound.setVolume(50); // 0-100
        engineSound.setLoop(true);
        // engineSound.play();
    }

    // Initialize game state
    playerX = 0;
    camX = 0;
    pos = 0;
    H = 1500;
    currentSpeed = 0;
    carAngle = 0.0f;

    // Build road lines (customize per mapIndex if needed)
    int segL = 200;
    int roadW = 2000;
    for (int i = 0; i < 1600; i++)
    {
        Line line;
        line.z = i * segL;

        if (i > 300 && i < 700)
            line.curve = 0.5;
        if (i > 1100)
            line.curve = -0.7;

        if (i < 300 && i % 20 == 0)
        {
            line.spriteX = -2.5;
            line.sprite = object[5];
        }
        if (i % 17 == 0)
        {
            line.spriteX = 2.0;
            line.sprite = object[6];
        }
        if (i > 300 && i % 20 == 0)
        {
            line.spriteX = -0.7;
            line.sprite = object[4];
        }
        if (i > 800 && i % 20 == 0)
        {
            line.spriteX = -1.2;
            line.sprite = object[1];
        }
        if (i == 400)
        {
            line.spriteX = -1.2;
            line.sprite = object[7];
        }

        if (i > 750)
            line.y = sin(i / 30.0) * 1500;

        lines.push_back(line);
    }
    N = lines.size();
}

void GameView::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
            escapePressed = true;
    }
}

void GameView::update()
{
    // Handle input (can be improved for smoother input)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        playerX += 0.1f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        playerX -= 0.1f;
    }

    // Speed logic
    const int MAX_FORWARD_SPEED = 1000;
    const int MAX_REVERSE_SPEED = -200;
    const int ACCELERATION = 1;
    const int BRAKE_DECELERATION = 5;
    const int NATURAL_DECELERATION = 5;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        currentSpeed += ACCELERATION;
        if (currentSpeed > MAX_FORWARD_SPEED)
            currentSpeed = MAX_FORWARD_SPEED;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (currentSpeed > 0)
        {
            currentSpeed -= BRAKE_DECELERATION;
            if (currentSpeed < 0)
                currentSpeed = 0;
        }
        else
        {
            currentSpeed -= ACCELERATION;
            if (currentSpeed < MAX_REVERSE_SPEED)
                currentSpeed = MAX_REVERSE_SPEED;
        }
    }
    else
    {
        if (currentSpeed > 0)
        {
            currentSpeed -= NATURAL_DECELERATION;
            if (currentSpeed < 0)
                currentSpeed = 0;
        }
        else if (currentSpeed < 0)
        {
            currentSpeed += NATURAL_DECELERATION;
            if (currentSpeed > 0)
                currentSpeed = 0;
        }
    }

    camX += (playerX * 2000 - camX) * 0.1f; // 2000 = roadW

    // After updating speed, control the engine sound:
    if (currentSpeed != 0)
    {
        if (engineSound.getStatus() != sf::Music::Playing)
            engineSound.play();
    }
    else
    {
        if (engineSound.getStatus() == sf::Music::Playing)
            engineSound.pause();
    }

    engineSound.setPitch(1.0f + std::abs(currentSpeed) * 0.001f);

    pos += currentSpeed;
    while (pos >= N * 200)
        pos -= N * 200; // 200 = segL
    while (pos < 0)
        pos += N * 200;
}

void GameView::drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1 - w1, y1));
    shape.setPoint(1, Vector2f(x2 - w2, y2));
    shape.setPoint(2, Vector2f(x2 + w2, y2));
    shape.setPoint(3, Vector2f(x1 + w1, y1));
    w.draw(shape);
}

void GameView::draw(sf::RenderWindow &window)
{
    window.clear(sf::Color(105, 205, 4));
    int segL = 200;
    int roadW = 2000;
    int startPos = pos / segL;
    int camH = lines[startPos].y + H;
    int maxy = height;
    float x = 0, dx = 0;

    window.clear(Color(105, 205, 4));
    window.draw(sBackground);

    if (currentSpeed > 0)
        sBackground.move(-lines[startPos].curve * 2, 0);
    if (currentSpeed < 0)
        sBackground.move(lines[startPos].curve * 2, 0);

    // Update speed text
    speedText.setString(std::to_string(std::abs(currentSpeed) / 10));         // Adjust divisor for display units
    speedText.setPosition(width - speedText.getLocalBounds().width - 40, 40); // Top-right with margin

    ///////draw road////////
    for (int n = startPos; n < startPos + 300; n++)
    {
        Line &l = lines[n % N];
        l.project(playerX * roadW - x, camH, startPos * segL - (n >= N ? N * segL : 0));
        // ...in your road projection loop, use camX instead of playerX*roadW...
        // l.project(camX - x, camH, startPos*segL - (n>=N?N*segL:0));
        x += dx;
        dx += l.curve;

        l.clip = maxy;
        if (l.Y >= maxy)
            continue;
        maxy = l.Y;

        Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
        Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
        Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

        Line p = lines[(n - 1) % N]; // previous line

        drawQuad(window, grass, 0, p.Y, width, 0, l.Y, width);
        drawQuad(window, rumble, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
        drawQuad(window, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
    }

    ////////draw objects////////
    for (int n = startPos + 300; n > startPos; n--)
        lines[n % N].drawSprite(window);

    float carScreenX = width / 2;
    float carScreenY = height * 0.8f;
    float carAngle = 0.0f;

    if (Keyboard::isKeyPressed(Keyboard::Left))
        carAngle = -3.0f;
    else if (Keyboard::isKeyPressed(Keyboard::Right))
        carAngle = 3.0f;

    playerCar.setAngle(carAngle);
    playerCar.setPosition(width / 2, height * 0.8f);
    playerCar.draw(window);

    // Draw the speedometer
    speedometer.setSpeed(static_cast<float>(std::abs(currentSpeed)));
    speedometer.draw(window, font);
}