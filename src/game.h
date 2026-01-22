#ifndef GAME_H
#define GAME_H

#include "player.h"

const std::string GAME_TITLE = "the game";

class Game {
public:
  Game();

  void main_loop();

private:
  bool is_running() const;

  void update();

  void handle_events();

  void handle_event(std::optional<sf::Event>);

  void handle_key_pressed(const sf::Event::KeyPressed *key_pressed);

  void quit();

  void draw();

  sf::RenderWindow window;
  Player player;
  World world;
  sf::Clock clock;
  // mt19937 is a random number generator
  std::mt19937 rng;
};

#endif
