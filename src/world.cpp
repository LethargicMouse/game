#include "world.h"

World::World() : shape({TILE_SIZE, TILE_SIZE}) {
  shape.setFillColor(sf::Color(0x888888));
}

void World::draw(sf::RenderWindow &window) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - TILE_SIZE_H,
                   (float)center.y - TILE_SIZE_H);
  shape.setPosition(pos);
  window.draw(shape);
}
