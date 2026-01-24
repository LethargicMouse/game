#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

inline constexpr float TILE_SIZE = 50;

enum class TileKind { Wall, Floor, Water };

class Tile {
public:
  Tile();
  Tile(TileKind kind, sf::Vector2i pos, float light);

  void draw(sf::RenderWindow &window, sf::Vector2f origin);

  bool is_wall() const;
  bool is_water() const;
  bool is_black() const;

  void set_light(float light);
  float get_light() const;

private:
  sf::RectangleShape shape;
  TileKind kind;
  float light;
  sf::Vector2f pos;
};

TileKind random_kind();

#endif
