#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

inline constexpr float TILE_SIZE = 100;

enum class TileKind { Wall, Floor };

class Tile {
public:
  Tile();
  Tile(TileKind kind, sf::Vector2i pos);

  void draw(sf::RenderWindow &window, sf::Vector2f origin);

  bool is_wall() const;

private:
  sf::RectangleShape shape;
  TileKind kind;
  sf::Vector2f pos;
};

#endif
