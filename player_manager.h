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

    [[nodiscard]] int get_player_lives() const {
        return player_lives;
    }

    [[nodiscard]] bool get_is_player_on_ground() const {
        return is_player_on_ground;
    }

    void set_is_player_on_ground(const bool on_ground) {
        is_player_on_ground = on_ground;
    }

    [[nodiscard]] bool get_is_looking_forward() const {
        return is_looking_forward;
    }

    [[nodiscard]] bool get_is_moving() const {
        return is_moving;
    }

    void set_is_moving(const bool is_moving) {
        this->is_moving = is_moving;
    }

    [[nodiscard]] int get_max_player_lives() const {
        return MAX_PLAYER_LIVES;
    }


private:
    bool is_player_on_ground{};
    bool is_looking_forward{};
    bool is_moving{};
    int player_level_scores[LEVEL_COUNT]{};
    const int MAX_PLAYER_LIVES = 3;
    int player_lives = MAX_PLAYER_LIVES;

    PlayerManager()
      :
        is_looking_forward(true),
        player({0, 0}, 0) {}
    ~PlayerManager() = default;

    Player player;
};


#endif //PLAYER_MANAGER_H
