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
    float time_left_over = 0.0f;
    float animation_elapsed = 0.0f;
    float frames_per_second = 60.0f;
    const int run_animation[2] = {0, 2};
    const int num_anim = 2;
    int current_anim_index = 0;
    
    Entity *player;
    GLuint emojis_texture;
    GLuint green_block_texture;
    GLuint red_block_texture;
    GLuint coin_texture;
    GLuint growblock_texture;
    GLuint font_texture;
    std::vector<Block*> blocks;
    std::vector<Enemy*> enemies;
    std::vector<Growblock*> growblocks;
    Coin *goal = new Coin();
    int lives;
    float startx;
    float starty;
    
    GameState state;
    
    bool cheatmode = false;
    bool finished_level = true;
    bool win = false;
    
    Mix_Chunk *hit;
    Mix_Chunk *kill;
    Mix_Chunk *die;
    Mix_Chunk *jump;
    Mix_Chunk *coin;
    Mix_Music *music;
    
public:
    Platformer();
    ~Platformer();
    
    void render_title_screen();
    void render_game_over_screen();
    
    void build_map();
    void render_map();
    
    bool growblock_is_collided(Growblock *g);
    
    void window_setup();
    void texture_setup();
    void process_events();
    void update(float elapsed);
    void render();
    bool run_game();
};

#endif /* Platformer_hpp */
