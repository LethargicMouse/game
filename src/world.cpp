#include "world.h"

World::World() { tiles.emplace_back(); }

void World::draw(sf::RenderWindow &window, const sf::Vector2f origin) {
  for (auto &tile : tiles)
    tile.draw(window, origin);
}
