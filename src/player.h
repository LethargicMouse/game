#ifndef PLAYER_H
#define PLAYER_H

#include "world.h"
#include <SFML/Graphics.hpp>

class Player {
public:
  Player();

  void draw(sf::RenderWindow &window);

  void update(sf::Time dt);

  sf::Vector2f get_pos() const;

private:
  sf::CircleShape shape;
  sf::Vector2f pos;
};

#endif
