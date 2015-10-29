//
//  Platformer.h
//  NYUCodebase
//

#ifndef PLATFORMER_H
#define PLATFORMER_H

#include "Utils.h"
#include "Entity.h"
#include <vector>
#include <fstream>
#include <iostream>

#define BLOCK_SIZE 1.0f

class Platformer
{
private:
    SDL_Window *display_window;
    SDL_Event event;
    bool done;
    ShaderProgram *program;
    Utilities::Utils utilities;

    Matrix projection_matrix;
    Matrix view_matrix;
    Matrix model_matrix;

    float last_frame_ticks;

    // platformer-specific objects
    Entity *player;
    float player_velocity = 10.0f;
    std::vector<Entity*> blocks;
    GLuint player_texture_id;
    GLuint blocks_texture_id;
    int map_width;
    int map_height;
    unsigned char **level_data;

public:
    Platformer();
    ~Platformer();

    // standard procedure
    void setup();
    void process_events();
    void update(float elapsed);
    void render();
    bool update_and_render();

    // platformer-specific functions
    void create_player();
    void build_level();
    bool read_header(std::ifstream &stream);
    bool read_layer_data(std::ifstream &stream);
    void render_blocks();
    void scroll_screen();
};

#endif
