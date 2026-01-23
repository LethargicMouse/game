#include "tile.h"

inline constexpr float TILE_SIZE_H = TILE_SIZE * 0.5;
inline constexpr sf::Color FLOOR_COLOR = sf::Color(0x88'88'88'ff);
inline constexpr sf::Color WALL_COLOR = sf::Color(0x22'22'22'ff);
inline constexpr sf::Color WATER_COLOR = sf::Color(0x99'dd'dd'ff);

Tile::Tile() : kind(TileKind::Wall) {}

sf::Color kind_color(TileKind kind) {
  switch (kind) {
  case TileKind::Floor:
    return FLOOR_COLOR;
  case TileKind::Wall:
    return WALL_COLOR;
  case TileKind::Water:
    return WATER_COLOR;
  }
  // i hate this language
  return sf::Color::Black;
}

inline constexpr std::uint8_t MAX_ALPHA = 255;

std::uint8_t alpha_from_dist(unsigned int dist) {
  float frac = 1.f - (float)dist / (float)DARK_DIST;
  frac = std::max(frac, 0.f);
  return (std::uint8_t)(MAX_ALPHA * frac);
}

Tile::Tile(TileKind kind, sf::Vector2i grid_pos, unsigned int dist)
    : shape({TILE_SIZE, TILE_SIZE}), kind(kind), color(kind_color(kind)),
      pos(sf::Vector2f((float)grid_pos.x, (float)grid_pos.y) * TILE_SIZE) {
  color.a = alpha_from_dist(dist);
  shape.setFillColor(color);
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
  default:
    return false;
  }
}

bool Tile::is_water() const {
  switch (kind) {
  case TileKind::Water:
    return true;
  default:
    return false;
  }
}

void Tile::set_dist(unsigned int dist) {
  color.a = alpha_from_dist(dist);
  shape.setFillColor(color);
}
