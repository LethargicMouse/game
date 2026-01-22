#ifndef PLAYER_H
#define PLAYER_H

#include "world.h"
#include <SFML/Graphics.hpp>

inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.5;

class Player {
public:
  Player();

  void draw(sf::RenderWindow &window);

private:
  sf::CircleShape shape;
};

#endif
