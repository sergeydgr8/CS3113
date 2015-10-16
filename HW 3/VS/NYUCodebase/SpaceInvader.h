//
//  SpaceInvader.h
//  NYUCodebase
//

#ifndef SpaceInvader_h
#define SpaceInvader_h

#include "Utils.h"
#include "Entity.h"
#include "Bullet.h"

#include <vector>
#include <algorithm>

class SpaceInvader
{
private:
    // window and game setup
    SDL_Window *display_window;
    SDL_Event event;
    bool done;
    ShaderProgram *program;
    Utils utilities; // made a separate file for texture loading, etc.

    // matrices
    Matrix projection_matrix;
    Matrix view_matrix;
    Matrix model_matrix;

    // general game
    GameState game_state;
    float last_frame_ticks;
    bool game_won;

    // space invaders specific
    Entity *player;
    std::vector<Entity*> invaders;
    std::vector<Bullet*> bullets;
    GLuint font_tex_id;
    GLuint bullet_tex_id;
    GLuint invaders_tex_id;
    GLuint player_tex_id;
    float attack_interval;
    float move_interval;

public:
    SpaceInvader();
    ~SpaceInvader();

    void setup();
    void make_invaders(std::vector<Entity*> &invaders, int num_invaders);
    void reset();

    void process_events(float elapsed);
    void update(float elapsed);

    void invader_shoots();
    void move_invaders(float elapsed);
    bool are_all_invaders_dead();
    bool is_colliding_with_bullet(Entity *entity, Bullet *bullet);
    void check_for_collisions();
    void clear_inactive_bullets();
    void check_for_win();

    void render_title_screen();
    void render_game();
    void render_game_over();
    void render();
    bool update_and_render();
};

#endif
