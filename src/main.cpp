#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "LoadingView.hpp"
#include "MenuView.hpp"
#include "SettingsView.hpp"
#include "GameView.hpp"

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

// Utility function for centered text
void centerText(sf::Text &text, float x, float y)
{
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(bounds.width / 2, bounds.height / 2);
  text.setPosition(x, y);
}

enum GameState
{
  LOADING,
  MENU,
  SETTINGS,
  GAME,
  QUIT
};

int main()
{
  ContextSettings settings;
  settings.antialiasingLevel = 8; // 4 or 8 for smooth edges
  RenderWindow app(sf::VideoMode(width, height), "Racing!", sf::Style::Titlebar | sf::Style::Close, settings);
  app.setFramerateLimit(60);

  // Add after loading textures, before main loop
  Font speedFont;
  if (!speedFont.loadFromFile("fonts/DS-DIGIT.ttf"))
  {
    std::cout << "Failed to load DS-DIGIT.ttf\n";
  }

  // --- Loading Screen ---
  GameState state = LOADING;
  vector<string> menuItems = {"Start Game", "Settings", "Quit"};
  vector<std::string> carOptions = {"Red Car", "Blue Car", "White Car"};
  vector<std::string> mapOptions = {"Classic", "Desert", "Snow"};

  int menuIndex = 0;

  // --- Settings Variables ---
  int carIndex = 0;
  int mapIndex = 0;
  int settingsIndex = 0;

  LoadingView loadingView(speedFont, width, height);
  MenuView menuView(speedFont, width, height);
  SettingsView settingsView(speedFont, width, height, carOptions, mapOptions);
  std::unique_ptr<GameView> gameView = std::make_unique<GameView>(speedFont, width, height, carIndex, mapIndex);

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

      // Pass event to current view
      if (state == MENU)
        menuView.handleEvent(e);
      if (state == SETTINGS)
        settingsView.handleEvent(e);
      if (state == GAME)
        gameView->handleEvent(e);
    }

    if (state == LOADING)
    {
      loadingView.draw(app);
      app.display();
      std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulate loading
      state = MENU;                                                 // Switch to menu after loading
    }
    else if (state == MENU)
    {
      menuView.draw(app);
      app.display();
      if (menuView.isEnterPressed())
      {
        int selected = menuView.getSelected();
        if (selected == 0) {
          gameView = std::make_unique<GameView>(speedFont, width, height, carIndex, mapIndex); // Recreate gameView
          state = GAME;
        } else if (selected == 1)
          state = SETTINGS;
        else if (selected == 2)
          state = QUIT;
        menuView.resetEnter();
      }
    }
    else if (state == SETTINGS)
    {
      settingsView.draw(app);
      app.display();
      if (settingsView.isBackSelected())
      {
        carIndex = settingsView.getCarIndex();
        state = MENU;
        settingsView.resetBack();
      }
    }
    else if (state == GAME)
    {
      gameView->update();
      gameView->draw(app);
      if (gameView->isEscapePressed())
      {
        state = MENU;
        gameView->resetEscape();
      }
      app.display();
    }
    else if (state == QUIT)
    {
      app.close();
    }
  }

  return 0;
}