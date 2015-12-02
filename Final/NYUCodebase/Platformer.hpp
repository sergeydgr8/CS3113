//
//  Platformer.hpp
//  NYUCodebase
//

#ifndef Platformer_hpp
#define Platformer_hpp

#include "Entity.hpp"

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
    
    Entity *player;
    GLuint emojis_texture;
    GLuint green_block_texture;
    GLuint red_block_texture;
    GLuint font_texture;
    std::vector<Block*> blocks;
    std::vector<Enemy*> enemies;
    
    GameState state;
    
    bool cheatmode;
    
public:
    Platformer();
    ~Platformer();
    
    void build_map();
    void render_map();
    void check_for_collisions();
    void scroll_screen();
    
    void window_setup();
    void texture_setup();
    void process_events(float elapsed);
    void update(float elapsed);
    void render();
    bool run_game();
};

#endif /* Platformer_hpp */
