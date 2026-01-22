#ifndef WORLD_H
#define WORLD_H

#include "tile.h"
#include <deque>
#include <map>

// required to use `std::map<sf::Vector2i, _>`
struct Vector2iComparator {
public:
  bool operator()(const sf::Vector2i a, const sf::Vector2i b) const;
};

class World {
public:
  World(const sf::Vector2f *player_pos);

  void draw(sf::RenderWindow &window, const sf::Vector2f origin);

  void update();

private:
  void regenerate_tiles();

  Tile new_tile(sf::Vector2i pos);

  std::map<sf::Vector2i, Tile, Vector2iComparator> tiles;
  const sf::Vector2f *player_pos;
  sf::Vector2i player_grid_pos;
  std::deque<sf::Vector2i> queue;
};

#endif
