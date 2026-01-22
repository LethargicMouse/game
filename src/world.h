#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

inline constexpr float TILE_SIZE = 100;
inline constexpr float TILE_SIZE_H = TILE_SIZE * 0.5;

class World {
public:
  World();

  void draw(sf::RenderWindow &window, const sf::Vector2f origin);

  sf::RectangleShape shape;
};

#endif
