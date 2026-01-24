#ifndef GAME_H
#define GAME_H

#include "tile.h"
#include <SFML/System/Vector2.hpp>
#include <deque>
#include <map>

// required to use `std::map<sf::Vector2i, _>`
struct Vector2iComparator {
public:
  bool operator()(sf::Vector2i left, sf::Vector2i right) const;
};

const std::string GAME_TITLE = "the game";

class Game {
public:
  Game();

  void main_loop();

private:
  bool is_running() const;
  void quit();

  void update();

  void update_player(sf::Time delta_time);
  void move_player(sf::Vector2f vector);

  void update_world();
  void regenerate_tiles();
  void erase_black_tiles();

  unsigned int get_distance(sf::Vector2i pos);

  void handle_events();
  void handle_event(std::optional<sf::Event>);
  void handle_key_pressed(const sf::Event::KeyPressed *key_pressed);

  void place_torch();

  void draw();
  void draw_player();
  void draw_world(sf::Vector2f origin);

  sf::RenderWindow window;

  sf::CircleShape player_shape;
  sf::Vector2f player_pos;
  sf::Vector2i player_grid_pos;
  sf::Vector2i old_player_grid_pos;

  std::map<sf::Vector2i, Tile, Vector2iComparator> tiles;

  std::vector<sf::Vector2i> light_sources;

  sf::Clock clock;
};

#endif
