#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

inline constexpr float TILE_SIZE = 100;
inline constexpr float TILE_SIZE_H = TILE_SIZE * 0.5;
inline constexpr unsigned int TILE_COLOR = 0x88'88'88'ff;

class Tile {
public:
  Tile();

  void draw(sf::RenderWindow &window, sf::Vector2f origin);

private:
  sf::RectangleShape shape;
};

#endif
