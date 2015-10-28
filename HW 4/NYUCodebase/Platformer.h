//
//  Platformer.h
//  NYUCodebase
//

#ifndef PLATFORMER_H
#define PLATFORMER_H

#include "Utils.h"
#include "Entity.h"


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

public:
    Platformer();
    ~Platformer();

    void setup();
    void process_events(float elapsed);
    void update(float elapsed);
    void render();
    bool update_and_render();
};

#endif
