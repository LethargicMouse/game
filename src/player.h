#ifndef PLAYER_H
#define PLAYER_H

#include "world.h"
#include <SFML/Graphics.hpp>

inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.5;
inline constexpr float PLAYER_SPEED = 100;

class Player {
public:
  Player();

  void draw(sf::RenderWindow &window);

  void update();

  sf::Vector2f get_pos() const;

private:
  sf::CircleShape shape;
  sf::Vector2f pos;
};

#endif
