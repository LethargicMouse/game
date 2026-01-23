#include "world.h"
#include "tile.h"
#include <SFML/System/Vector2.hpp>
#include <random>

bool Vector2iComparator::operator()(const sf::Vector2i a,
                                    const sf::Vector2i b) const {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

World::World(const sf::Vector2i *player_grid_pos)
    : player_grid_pos(player_grid_pos), old_player_grid_pos(*player_grid_pos) {
  tiles[*player_grid_pos] = Tile(TileKind::Floor, *player_grid_pos, 0);
  queue.push_back(*player_grid_pos);
  regenerate_tiles();
}

const std::array<sf::Vector2i, 4> DIRS({{0, 1}, {1, 0}, {-1, 0}, {0, -1}});

// behold, the SINGLETON! (mini version)
std::mt19937 &get_rng() {
  // don't wanna know who invented this {}() bullshit
  // mt19937 is a smart-ass pseudo random number generator
  static std::mt19937 rng(std::random_device{}());
  return rng;
}

template <typename T> T real_random_between(T min, T max) {
  std::uniform_real_distribution<T> dist(min, max);
  return dist(get_rng());
}

// squared distance between Vector2i's
unsigned int distance2i(const sf::Vector2i a, const sf::Vector2i b) {
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline constexpr float WALL_PROB_BORDER = 0.4;

Tile World::new_tile(sf::Vector2i pos) {
  // get random number in given distribution
  // C++ is fucking garbage of a language
  float number = real_random_between(0.f, 1.f);
  TileKind kind = TileKind::Floor;
  if (number <= WALL_PROB_BORDER)
    kind = TileKind::Wall;
  return Tile(kind, pos, distance2i(*player_grid_pos, pos));
}

// Purest, first-class cosmic horror, for the sake of your young and innocent
// brain dont even try to understand what the freaking hell is happening here,
// for God left this place long ago
void World::regenerate_tiles() {
  std::vector<sf::Vector2i> new_queue;
  for (auto &[pos, tile] : tiles) {
    auto dist = distance2i(*player_grid_pos, pos);
    if (dist > DARK_DIST)
      new_queue.push_back(pos); // diabolically reusing vectors
    else
      tile.set_dist(dist);
  }
  for (auto pos : new_queue) {
    tiles.erase(pos);
    for (auto dir : DIRS) {
      auto neigh = pos + dir;
      if (tiles.contains(neigh))
        queue.push_back(neigh); // they are now on border
    }
  }
  new_queue.clear();

  while (!queue.empty()) {
    auto pos = queue.front();
    queue.pop_front();
    if (!tiles.contains(pos))
      continue; // a friendly ghost visited this haunted code, just skip it
    // not generating past walls and such cuz they're not transparent
    if (tiles[pos].is_wall())
      continue;
    bool in_new_queue = false;
    for (auto dir : DIRS) {
      auto neighbour = pos + dir;
      if (tiles.contains(neighbour))
        continue;
      if (distance2i(*player_grid_pos, neighbour) > DARK_DIST) {
        if (!in_new_queue) {
          // tile is on border, adding to stash
          in_new_queue = true;
          new_queue.push_back(pos);
        }
        continue;
      }
      tiles[neighbour] = new_tile(neighbour);
      queue.push_back(neighbour);
    }
  }
  // stash border tiles to regenerate starting from them next time
  for (auto &pos : new_queue)
    queue.push_back(pos);
}

void World::draw(sf::RenderWindow &window, const sf::Vector2f origin) {
  for (auto &[_, tile] : tiles)
    tile.draw(window, origin);
}

void World::update() {
  if (*player_grid_pos != old_player_grid_pos)
    regenerate_tiles();
  old_player_grid_pos = *player_grid_pos;
}

// Tile should be present on this pos
auto World::get_tile(const sf::Vector2i pos) -> Tile const& { return tiles[pos]; }