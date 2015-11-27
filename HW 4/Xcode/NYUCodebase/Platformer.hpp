//
//  Platformer.hpp
//  NYUCodebase
//

#ifndef Platformer_hpp
#define Platformer_hpp

#include "Entity.hpp"

#define TILE_SIZE 0.75f

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
    
    // platformer specific
    Entity *player;
    GLuint character_texture;
    GLuint block_texture;
    int map_width;
    int map_height;
    unsigned char **level_data;
    std::vector<Block*> blocks;
    //float tile_size;
    
public:
    Platformer();
    ~Platformer();
    
    bool read_header(std::ifstream &stream);
    bool read_layer_data(std::ifstream &stream);
    
    void build_map();
    void render_map();
    void check_for_collisions();
    void scroll_screen();
    
    void setup();
    void process_events(float elapsed);
    void update(float elapsed);
    void render();
    bool run_game();
};

#endif /* Platformer_hpp */
