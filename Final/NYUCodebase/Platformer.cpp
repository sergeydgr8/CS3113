//
//  Platformer.cpp
//  NYUCodebase
//

#include "Platformer.hpp"

Platformer::Platformer()
{
    cheatmode = true; // uncomment this to jump to a certain level from title screen
    state = TITLE_SCREEN;
    window_setup();
    texture_setup();
}

Platformer::~Platformer()
{
    delete program;
    for (size_t i = 0; i < blocks.size(); i++)
        delete blocks[i];
    blocks.clear();
    for (size_t i = 0; i < enemies.size(); i++)
        delete enemies[i];
    enemies.clear();
    SDL_Quit();
}

void Platformer::render_title_screen()
{
    model_matrix.identity();
    model_matrix.Translate(-5.0f, 3.7f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "EMOJI PLATFORMER", 1.0f, -0.3f);
    
    model_matrix.identity();
    model_matrix.Translate(-4.1f, 2.9f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "NYU-Poly CS-UY 3113 Project by Sergey Smirnov", 0.3f, -0.1f);
    
    model_matrix.identity();
    model_matrix.Translate(-1.7f, 2.2f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "INSTRUCTIONS:", 0.5f, -0.2f);
    
    Utilities::SheetSprite emojis(emojis_texture, 8.0f, 1.0f, 1.0f, 1.0f, 0.75f);
    
    emojis.Draw(program, model_matrix, 0, -5.5f, 1.2f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 1.2f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is YOU. Use your left and right arrow keys to move, up arrow key to jump.", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 4, -5.5f, 0.2f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 0.35f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is one of your enemies. You can beat them if you're larger, otherwise you", 0.3f, -0.16f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 0.1f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "die upon contact.", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 6, -5.5f, -0.8f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -0.8f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is another enemy. These follow you and you cannot beat them.", 0.3f, -0.16f);
    
    
}

// TODO: Build all the levels
void Platformer::build_map()
{
    // clear out all the blocks and enemies to create new level
    for (size_t i = 0; i < blocks.size(); i++)
        delete blocks[i];
    blocks.clear();
    for (size_t i = 0; i < enemies.size(); i++)
        delete enemies[i];
    enemies.clear();
    
    switch (state)
    {
        case LEVEL1:
            blocks.push_back(new Block(-5.5f, -3.0f, 0.2f, 0.8f, 0));
            blocks.push_back(new Block(-4.5f, -2.7f, 0.2f, 1.5f, 0));
            for (Block *b : blocks)
            {
                if (b->get_type() == 0)
                    b->set_sprite(green_block_texture, 1, 1);
                else if (b->get_type() == 1)
                    b->set_sprite(red_block_texture, 1, 1);
                b->render(program, model_matrix, 0);
            }
            break;
        case LEVEL2:
            // render level 2
            break;
        case LEVEL3:
            // render level 3
            break;
        case LEVEL4:
            // render level 4
            break;
        case LEVEL5:
            // render level 5
            break;
        case LEVEL6:
            // render level 6
            break;
        case LEVEL7:
            // render level 7
            break;
        case LEVEL8:
            // render level 8
            break;
        case LEVEL9:
            // render level 9
            break;
        case LEVEL10:
            // render level 10
            break;
        default:
            break;
    }
}

void Platformer::check_for_collisions()
{
    for (Block *b : blocks)
        player->is_colliding_with(b);
}

void Platformer::window_setup()
{
    try
    {
        SDL_Init(SDL_INIT_VIDEO);
        display_window = SDL_CreateWindow("Emoji Platformer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1280, 720,
                                          SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(display_window);
        SDL_GL_MakeCurrent(display_window, context);
#ifdef _WINDOWS
        glewInit();
#endif
        
        glViewport(0, 0, 1280, 720);
        
        program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl",
                                    RESOURCE_FOLDER"fragment_textured.glsl");
        
        projection_matrix.setOrthoProjection(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        view_matrix.identity();
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        done = false;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in window_setup(): " << e.what() << "\n";
    }
    
}

void Platformer::texture_setup()
{
    emojis_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"emojis.png");
    green_block_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"green_block.png");
    red_block_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"red_block.png");
    font_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"font1.png");
}

void Platformer::process_events()
{
    try
    {
        while (SDL_PollEvent(&event))
        {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            {
                done = true;
            }
            if (state == TITLE_SCREEN)
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_RETURN ||
                        event.key.keysym.scancode == SDL_SCANCODE_RETURN2 ||
                        event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                        state = LEVEL1;
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        done = true;
                    if (cheatmode) // for debug purposes
                    {
                        if (event.key.keysym.scancode == SDL_SCANCODE_1)
                            state = LEVEL1;
                        if (event.key.keysym.scancode == SDL_SCANCODE_2)
                            state = LEVEL2;
                        if (event.key.keysym.scancode == SDL_SCANCODE_3)
                            state = LEVEL3;
                        if (event.key.keysym.scancode == SDL_SCANCODE_4)
                            state = LEVEL4;
                        if (event.key.keysym.scancode == SDL_SCANCODE_5)
                            state = LEVEL5;
                        if (event.key.keysym.scancode == SDL_SCANCODE_6)
                            state = LEVEL6;
                        if (event.key.keysym.scancode == SDL_SCANCODE_7)
                            state = LEVEL7;
                        if (event.key.keysym.scancode == SDL_SCANCODE_8)
                            state = LEVEL8;
                        if (event.key.keysym.scancode == SDL_SCANCODE_9)
                            state = LEVEL9;
                        if (event.key.keysym.scancode == SDL_SCANCODE_0)
                            state = LEVEL10;
                    }
                }
            }
            else if (state == LEVEL1 || state == LEVEL2 || state == LEVEL3 ||
                     state == LEVEL4 || state == LEVEL5 || state == LEVEL6 ||
                     state == LEVEL7 || state == LEVEL8 || state == LEVEL9 ||
                     state == LEVEL10)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    state = TITLE_SCREEN;
                if (keys[SDL_SCANCODE_LEFT])
                    player->move(-5.0f);
                if (keys[SDL_SCANCODE_RIGHT])
                    player->move(5.0f);
                if (keys[SDL_SCANCODE_UP])
                    if (!player->has_jumped())
                        player->jump();
                player->move(0.0f);
            }
            else if (state == GAME_OVER)
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_RETURN ||
                        event.key.keysym.scancode == SDL_SCANCODE_RETURN2 ||
                        event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                        state = TITLE_SCREEN;
                }
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in process_events(): " << e.what() << "\n";
    }
    
}

void Platformer::update(float elapsed)
{
    process_events();
    //player->update(elapsed);
    //check_for_collisions();
}

void Platformer::render()
{
    try
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program->programID);
        
        
        program->setProjectionMatrix(projection_matrix);
        program->setViewMatrix(view_matrix);
        
        if (state == TITLE_SCREEN)
        {
            render_title_screen();
        }
        else if (state == GAME_OVER)
        {
            // render game over screen
        }
        else
        {
            build_map();
        }
        //render_map();
        //player->render(program, model_matrix, 0);
        
        SDL_GL_SwapWindow(display_window);
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in render(): " << e.what() << "\n";
    }
    
}

bool Platformer::run_game()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - last_frame_ticks;
    last_frame_ticks = ticks;
    
    update(elapsed);
    render();
    return done;
}