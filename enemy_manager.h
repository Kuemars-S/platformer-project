#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"

#include <vector>
#include <raylib.h>

class EnemiesManager {
public:
    static EnemiesManager& getInstance() {
        static EnemiesManager instance;
        return instance;
    }
    EnemiesManager(const EnemiesManager&) = delete;
    EnemiesManager(EnemiesManager&&) = delete;
    EnemiesManager& operator=(const EnemiesManager&) = delete;
    EnemiesManager& operator=(EnemiesManager&&) = delete;

    void spawn_enemies();
    void update_enemies();
    [[nodiscard]] bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);

    [[nodiscard]] std::vector<Enemy> get_enemies() const {
        return enemies;
    }

private:
    EnemiesManager() = default;
    ~EnemiesManager() = default;

    std::vector<Enemy> enemies{};
};



#endif //ENEMY_MANAGER_H
