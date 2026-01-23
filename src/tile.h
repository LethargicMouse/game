#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

inline constexpr float TILE_SIZE = 50;
// on such squared distance tile will not be visible
inline constexpr unsigned int DARK_DIST = 20;
const float DARK_DIST_SQRT = std::sqrt((float)DARK_DIST);

enum class TileKind { Wall, Floor, Water };

class Tile {
public:
  Tile();
  Tile(TileKind kind, sf::Vector2i pos, unsigned int dist);

  void draw(sf::RenderWindow &window, sf::Vector2f origin);

  bool is_wall() const;
  bool is_water() const;
  bool is_black() const;

  // update based on new squared distance to the player
  void set_dist(unsigned int dist);

private:
  sf::RectangleShape shape;
  TileKind kind;
  sf::Color color;
  sf::Vector2f pos;
};

TileKind random_kind();

#endif
