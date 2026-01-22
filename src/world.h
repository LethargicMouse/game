#ifndef WORLD_H
#define WORLD_H

#include "tile.h"

class World {
public:
  World();

  void draw(sf::RenderWindow &window, const sf::Vector2f origin);

private:
  std::vector<Tile> tiles;
};

#endif
