#ifndef LEVEL_H
#define LEVEL_H

#include "enemy_manager.h"
#include "globals.h"
#include "player_manager.h"

class Level {
public:
    static Level& getInstance() {
        static Level instance;
        return instance;
    }
    Level(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(const Level&) = delete;
    Level& operator=(Level&&) = delete;

    bool is_inside_level(int row, int column) const;
    bool is_colliding(Vector2 pos, char look_for = '#');
    char& get_collider(Vector2 pos, char look_for);

    void reset_level_index();
    void load_level(int offset = 0);
    void unload_level() const;

    char& get_level_cell(size_t row, size_t column) const;
    void set_level_cell(size_t row, size_t column, char chr) const;

    //Getters and Setter
    level& get_current_level() {
        return current_level;
    }
    level& set_current_level(const level& lvl) {
        return current_level = lvl;
    }
private:
    Level() = default;
    ~Level() = default;
    level current_level;
    char *current_level_data = nullptr;
};

#endif //LEVEL_H
