//
// Created by windows 10 on 5/20/2025.
//

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H



#include "enemy_manager.h"
#include "globals.h"
#include "player_manager.h"

class LevelManager {
public:
    static LevelManager& getInstance() {
        static LevelManager instance;
        return instance;
    }
    LevelManager(const LevelManager&) = delete;
    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;

    bool is_inside_level(int row, int column) const;
    bool is_colliding(Vector2 pos, char look_for = '#');
    char& get_collider(Vector2 pos, char look_for);

    void reset_level_index();
    void load_level(int offset = 0);
    void unload_level() const;

    char& get_level_cell(size_t row, size_t column) const;
    void set_level_cell(size_t row, size_t column, char chr) const;

    Level& get_current_level() {
        return current_level;
    }
    Level& set_current_level(const Level& lvl) {
        return current_level = lvl;
    }
private:
    LevelManager() = default;
    ~LevelManager() = default;
    Level current_level;
    char *current_level_data = nullptr;
};



#endif //LEVEL_MANAGER_H
