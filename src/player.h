#ifndef PLAYER_H
#define PLAYER_H

#include "world.h"
#include <SFML/Graphics.hpp>

class Player {
public:
  Player(World* world);

  void draw(sf::RenderWindow &window);

  void update(sf::Time dt);

  sf::Vector2f const &get_pos() const;

  sf::Vector2i const &get_grid_pos() const;

private:
  sf::CircleShape shape;
  sf::Vector2f pos;
  sf::Vector2i grid_pos;
  // const but C++ is trash
  World *world;
};

#endif
