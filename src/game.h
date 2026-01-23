#ifndef GAME_H
#define GAME_H

#include "tile.h"
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

  void update();

  void handle_events();

  void handle_event(std::optional<sf::Event>);

  void handle_key_pressed(const sf::Event::KeyPressed *key_pressed);

  void quit();

  void draw();

  void draw_player();

  void update_player(sf::Time delta_time);

  void draw_world(sf::Vector2f origin);

  void regenerate_tiles();

  Tile new_tile(sf::Vector2i pos);

  void update_world();

  void move_player(sf::Vector2f vector);

  void erase_black_tiles();

  sf::RenderWindow window;

  sf::CircleShape player_shape;
  sf::Vector2f player_pos;
  sf::Vector2i player_grid_pos;
  sf::Vector2i old_player_grid_pos;

  std::map<sf::Vector2i, Tile, Vector2iComparator> tiles;
  std::deque<sf::Vector2i> tile_queue;

  sf::Clock clock;
};

#endif
