#include "tile.h"

inline constexpr float TILE_SIZE_H = TILE_SIZE * 0.5;
inline constexpr sf::Color FLOOR_COLOR = sf::Color(0x88'88'88'ff);
inline constexpr sf::Color WALL_COLOR = sf::Color(0x55'55'55'ff);

Tile::Tile() : kind(TileKind::Wall) {}

sf::Color kind_color(TileKind kind) {
  switch (kind) {
  case TileKind::Floor:
    return FLOOR_COLOR;
  case TileKind::Wall:
    return WALL_COLOR;
  }
  // i hate this language
  return sf::Color::Black;
}

Tile::Tile(TileKind kind, sf::Vector2i grid_pos)
    : shape({TILE_SIZE, TILE_SIZE}), kind(kind) {
  shape.setFillColor(kind_color(kind));
  pos = sf::Vector2f((float)grid_pos.x, (float)grid_pos.y) * TILE_SIZE;
}

void Tile::draw(sf::RenderWindow &window, sf::Vector2f origin) {
  auto center = window.getSize() / 2u;
  sf::Vector2f shift((float)center.x - TILE_SIZE_H,
                     (float)center.y - TILE_SIZE_H);
  shape.setPosition(shift + pos - origin);
  window.draw(shape);
}

bool Tile::is_wall() const {
  switch (kind) {
  case TileKind::Wall:
    return true;
  case TileKind::Floor:
    return false;
  }
  // unreachable but compiler doesn't fucking care
  return false;
}
