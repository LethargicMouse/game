#ifndef WORLD_H
#define WORLD_H

#include "tile.h"
#include <map>
#include <random>

// required to use `std::map<sf::Vector2i, _>`
struct Vector2iComparator {
public:
  bool operator()(const sf::Vector2i a, const sf::Vector2i b) const;
};

class World {
public:
  World(std::mt19937 *rng);

  void draw(sf::RenderWindow &window, const sf::Vector2f origin);

private:
  void regenerate_tiles();

  Tile new_tile(sf::Vector2i pos);

  std::map<sf::Vector2i, Tile, Vector2iComparator> tiles;
  // bullshit, not uniform at ALL
  // GOD I hate C++
  std::uniform_int_distribution<unsigned int> dist;
  // mt19937 is a random number generator
  std::mt19937 *rng;
};

#endif
