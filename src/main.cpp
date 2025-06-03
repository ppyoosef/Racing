#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "Speedometer.hpp"
#include "Car.hpp"
#include "Line.hpp"

using namespace sf;
using namespace std;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200;    // segment length
float camD = 0.84; // camera depth

const int MAX_FORWARD_SPEED = 1000;
const int MAX_REVERSE_SPEED = -200;
const int ACCELERATION = 2;
const int BRAKE_DECELERATION = 5;
const int NATURAL_DECELERATION = 4;

void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
  ConvexShape shape(4);
  shape.setFillColor(c);
  shape.setPoint(0, Vector2f(x1 - w1, y1));
  shape.setPoint(1, Vector2f(x2 - w2, y2));
  shape.setPoint(2, Vector2f(x2 + w2, y2));
  shape.setPoint(3, Vector2f(x1 + w1, y1));
  w.draw(shape);
}

int main()
{
  ContextSettings settings;
  settings.antialiasingLevel = 8; // 4 or 8 for smooth edges
  RenderWindow app(sf::VideoMode(width, height), "Racing!", sf::Style::Default, settings);
  app.setFramerateLimit(60);

  Texture t[50];
  Sprite object[50];
  for (int i = 1; i <= 7; i++)
  {
    t[i].loadFromFile("images/" + std::to_string(i) + ".png");
    t[i].setSmooth(true);
    object[i].setTexture(t[i]);
  }

  Texture bg;
  bg.loadFromFile("images/bg.png");
  bg.setRepeated(true);
  Sprite sBackground(bg);
  sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
  sBackground.setPosition(-2000, 0);

  // --- Add player car texture and sprite ---
  Car playerCar("images/car.png", width / 2, height * 0.8f);

  // --- Load and play engine sound ---
  Music engineSound;
  if (!engineSound.openFromFile("sounds/engine.ogg")) {
    std::cout << "Failed to load engine.ogg\n";
  } else {
    engineSound.setVolume(50); // 0-100
    engineSound.setLoop(true);
    engineSound.play();
  }

  // Add after loading textures, before main loop
  Font speedFont;
  if (!speedFont.loadFromFile("fonts/DS-DIGIT.ttf")) {
    std::cout << "Failed to load DS-DIGIT.ttf\n";
  }

  Text speedText;
  speedText.setFont(speedFont);
  speedText.setCharacterSize(96);
  speedText.setFillColor(Color::Red);
  speedText.setStyle(Text::Bold);

  Speedometer speedometer(width - 150, 150, speedFont);

  std::vector<Line> lines;

  for (int i = 0; i < 1600; i++) {
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

  int N = lines.size();
  float playerX = 0;
  int pos = 0;
  int H = 1500;

  float camX = 0.0f;
  while (app.isOpen())
  {
    Event e;
    while (app.pollEvent(e))
    {
      if (e.type == Event::Closed)
        app.close();
    }

    int speed = 0;
    static int currentSpeed = 0;

    if (Keyboard::isKeyPressed(Keyboard::Right))
      playerX += 0.1;
    if (Keyboard::isKeyPressed(Keyboard::Left))
      playerX -= 0.1;
    // if (Keyboard::isKeyPressed(Keyboard::Up)) speed=200;
    // if (Keyboard::isKeyPressed(Keyboard::Down)) speed=-200;
    // if (Keyboard::isKeyPressed(Keyboard::Tab)) speed*=3;
    // if (Keyboard::isKeyPressed(Keyboard::W)) H+=100;
    // if (Keyboard::isKeyPressed(Keyboard::S)) H-=100;

    // Acceleration and braking logic
    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
      currentSpeed += ACCELERATION;
      if (currentSpeed > MAX_FORWARD_SPEED)
        currentSpeed = MAX_FORWARD_SPEED;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down))
    {
      if (currentSpeed > 0)
      {
        currentSpeed -= BRAKE_DECELERATION; // Braking
        if (currentSpeed < 0)
          currentSpeed = 0;
      }
      else
      {
        currentSpeed -= ACCELERATION; // Reverse
        if (currentSpeed < MAX_REVERSE_SPEED)
          currentSpeed = MAX_REVERSE_SPEED;
      }
    }
    else
    {
      // Natural deceleration (friction)
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

    // ...inside your main loop, after updating playerX...
    camX += (playerX * roadW - camX) * 0.1f; // 0.1f is the smoothing factor

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

    // pos+=speed;
    pos += currentSpeed;

    while (pos >= N * segL)
      pos -= N * segL;
    while (pos < 0)
      pos += N * segL;

    app.clear(Color(105, 205, 4));
    app.draw(sBackground);
    int startPos = pos / segL;
    int camH = lines[startPos].y + H;
    if (speed > 0)
      sBackground.move(-lines[startPos].curve * 2, 0);
    if (speed < 0)
      sBackground.move(lines[startPos].curve * 2, 0);

    int maxy = height;
    float x = 0, dx = 0;

    // Update speed text
    speedText.setString(std::to_string(std::abs(currentSpeed) / 10));         // Adjust divisor for display units
    speedText.setPosition(width - speedText.getLocalBounds().width - 40, 40); // Top-right with margin

    // app.draw(speedText);

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

      drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
      drawQuad(app, rumble, p.X, p.Y, p.W * 1.2, l.X, l.Y, l.W * 1.2);
      drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
    }

    ////////draw objects////////
    for (int n = startPos + 300; n > startPos; n--)
      lines[n % N].drawSprite(app);

    float carScreenX = width / 2;
    float carScreenY = height * 0.8f;
    float carAngle = 0.0f;

    if (Keyboard::isKeyPressed(Keyboard::Left))
      carAngle = -3.0f;
    else if (Keyboard::isKeyPressed(Keyboard::Right))
      carAngle = 3.0f;

    playerCar.setAngle(carAngle);
    playerCar.setPosition(width / 2, height * 0.8f);
    playerCar.draw(app);

    // Draw the speedometer
    speedometer.setSpeed(static_cast<float>(std::abs(currentSpeed)));
    speedometer.draw(app, speedFont);

    app.display();
  }

  return 0;
}