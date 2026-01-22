#include "world.h"
#include <SFML/System/Vector2.hpp>
#include <deque>
#include <set>

inline constexpr unsigned int MAX_DEPTH = 7;

bool Vector2iComparator::operator()(const sf::Vector2i a,
                                    const sf::Vector2i b) const {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

sf::Vector2i grid_pos(sf::Vector2f pos) {
  return {(int)round(pos.x / TILE_SIZE), (int)round(pos.y / TILE_SIZE)};
}

World::World(const sf::Vector2f *player_pos)
    : player_pos(player_pos), player_grid_pos(grid_pos(*player_pos)) {
  tiles[player_grid_pos] = Tile(TileKind::Floor, player_grid_pos);
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

inline constexpr float WALL_PROB_BORDER = 0.2;

Tile World::new_tile(sf::Vector2i pos) {
  // get random number in given distribution
  // C++ is fucking garbage of a language
  float number = real_random_between(0.f, 1.f);
  TileKind kind = TileKind::Floor;
  if (number <= WALL_PROB_BORDER)
    kind = TileKind::Wall;
  return Tile(kind, pos);
}

void World::regenerate_tiles() {
  std::deque<std::pair<sf::Vector2i, unsigned int>> queue;
  std::set<sf::Vector2i, Vector2iComparator> used;
  queue.push_back({player_grid_pos, 0});
  while (!queue.empty()) {
    auto [pos, depth] = queue.front();
    queue.pop_front();
    used.insert(pos);
    // not generating past walls and such cuz they're not transparent
    if (tiles[pos].is_wall())
      continue;
    if (depth == MAX_DEPTH) {
      for (auto dir : DIRS) {
        auto neighbour = pos + dir;
        if (used.contains(neighbour))
          continue;
        auto tile = tiles.find(neighbour);
        if (tile != tiles.end())
          tiles.erase(tile);
      }
      continue;
    }
    for (auto dir : DIRS) {
      auto neighbour = pos + dir;
      if (used.contains(neighbour))
        continue;
      if (!tiles.contains(neighbour))
        tiles[neighbour] = new_tile(neighbour);
      queue.push_back({neighbour, depth + 1});
    }
  }
}

void World::draw(sf::RenderWindow &window, const sf::Vector2f origin) {
  for (auto &[_, tile] : tiles)
    tile.draw(window, origin);
}

void World::update() {
  auto new_player_grid_pos = grid_pos(*player_pos);
  if (new_player_grid_pos != player_grid_pos) {
    player_grid_pos = new_player_grid_pos;
    regenerate_tiles();
  }
}
