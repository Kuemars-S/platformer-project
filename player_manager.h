#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "globals.h"
#include "player.h"

class PlayerManager {
public:
    static PlayerManager& getInstance() {
        static PlayerManager instance;
        return instance;
    }
    PlayerManager(const PlayerManager&) = delete;
    PlayerManager(PlayerManager&&) = delete;
    PlayerManager& operator=(const PlayerManager&) = delete;
    PlayerManager& operator=(PlayerManager&&) = delete;

    void reset_player_stats();
    void increment_player_score();
    int get_total_player_score() const;

    void spawn_player();
    void kill_player();

    void move_player_horizontally(float delta);
    void update_player();
    void update_player_gravity();
    void try_jump();

    [[nodiscard]] Player get_player() const {
        return player;
    }

    bool is_player_on_ground{};
    bool is_looking_forward{};
    bool is_moving{};

    int player_level_scores[LEVEL_COUNT]{};

    const int MAX_PLAYER_LIVES = 3;
    int player_lives = MAX_PLAYER_LIVES;

private:
    PlayerManager()
      :
        is_looking_forward(true),
        player({0, 0}, 0) {}
    ~PlayerManager() = default;

    Player player;
};


#endif //PLAYER_MANAGER_H
