#ifndef GAME_H
#define GAME_H

#include "world.h"

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

  void draw_player();

  void update_player(sf::Time dt);

  sf::RenderWindow window;
  sf::CircleShape player_shape;
  sf::Vector2f player_pos;
  sf::Vector2i player_grid_pos;
  World world;
  sf::Clock clock;
};

#endif
