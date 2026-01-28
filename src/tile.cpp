#include "tile.h"
#include <random>

inline constexpr float TILE_SIZE_H = TILE_SIZE * 0.5;
inline constexpr sf::Color FLOOR_COLOR = sf::Color(0x88'88'88'ff);
inline constexpr sf::Color WALL_COLOR = sf::Color(0x22'22'22'ff);
inline constexpr sf::Color WATER_COLOR = sf::Color(0x99'dd'dd'ff);

Tile::Tile() : kind(TileKind::Wall), light(0) {}

// helpers are conventionally defined in anonymous namespace so that they are
// accessible only in this .cpp file
namespace {
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
inline constexpr float MAX_LIGHT = 10;

std::uint8_t alpha_from_light(const float light) {
  return (std::uint8_t)(MAX_ALPHA * std::min(light, MAX_LIGHT) / MAX_LIGHT);
}
} // namespace

Tile::Tile(TileKind kind, sf::Vector2i grid_pos, float light)
    : shape({TILE_SIZE, TILE_SIZE}), kind(kind), light(light),
      pos(sf::Vector2f((float)grid_pos.x, (float)grid_pos.y) * TILE_SIZE) {
  auto color = kind_color(kind);
  color.a = alpha_from_light(light);
  shape.setFillColor(color);
}

void Tile::draw(sf::RenderWindow &window, sf::Vector2f origin) {
  auto center = window.getSize() / 2U;
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

void Tile::set_light(float light) {
  auto color = shape.getFillColor();
  color.a = alpha_from_light(light);
  shape.setFillColor(color);
  this->light = light;
}

float Tile::get_light() const { return light; }

namespace {
// behold, the SINGLETON! (mini version)
std::mt19937 &get_rng() {
  // don't wanna know who invented this {}() bullshit
  // mt19937 is a smart-ass pseudo random number generator
  static std::mt19937 rng(std::random_device{}());
  return rng;
}

template <typename T> T real_random_between(T min, T max) {
  std::uniform_real_distribution<T> dist(min, max);
  return dist(get_rng());
}
} // namespace

// dont forget to update `NOT_FLOOR_PROB` if you add new tile kind
inline constexpr float WATER_PROB = 0.1;
inline constexpr float WALL_PROB = 0.3;
inline constexpr float FLOOR_PROB = 0.6;
inline constexpr float NOT_FLOOR_PROB = WATER_PROB + WALL_PROB;
inline constexpr float EPS = 1e-6;

TileKind random_kind() {
  static_assert(std::abs(FLOOR_PROB + NOT_FLOOR_PROB - 1.F) <= EPS);
  float number = real_random_between(0.F, 1.F);
  // doing a bit of clever math here   (no)
  if (number <= WATER_PROB)
    return TileKind::Water;
  number -= WATER_PROB;
  if (number <= WALL_PROB)
    return TileKind::Wall;
  return TileKind::Floor;
}

bool Tile::is_black() const { return light == 0; }
