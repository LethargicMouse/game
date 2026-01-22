#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

const float PLAYER_RADIUS = 50.f;

class Player {
public:
  Player();

  void draw(sf::RenderWindow &window);

private:
  sf::CircleShape shape;
};

#endif
