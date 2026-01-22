#include "tile.h"

Tile::Tile() : shape({TILE_SIZE, TILE_SIZE}) {
  shape.setFillColor(sf::Color(TILE_COLOR));
}

void Tile::draw(sf::RenderWindow &window, sf::Vector2f origin) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - TILE_SIZE_H,
                   (float)center.y - TILE_SIZE_H);
  shape.setPosition(pos - origin);
  window.draw(shape);
}
