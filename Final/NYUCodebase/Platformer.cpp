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
    startx = -5.5f;
    starty = -3.0f;
    player = new Entity(startx, starty, 0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f);
    player->set_sprite(emojis_texture, 8.0f, 1.0f);
    lives = 5;
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
    finished_level = true;
    model_matrix.identity();
    model_matrix.Translate(-5.0f, 4.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "EMOJI PLATFORMER", 1.0f, -0.3f);
    
    model_matrix.identity();
    model_matrix.Translate(-4.1f, 3.3f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "NYU-Poly CS-UY 3113 Project by Sergey Smirnov", 0.3f, -0.1f);
    
    model_matrix.identity();
    model_matrix.Translate(-1.7f, 2.7f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "INSTRUCTIONS:", 0.5f, -0.2f);
    
    Utilities::SheetSprite emojis(emojis_texture, 8.0f, 1.0f, 1.0f, 1.0f, 0.75f);
    
    emojis.Draw(program, model_matrix, 0, -5.5f, 2.1f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 2.1f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is YOU. Use your left and right arrow keys to move, up arrow key to jump.", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 4, -5.5f, 1.1f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 1.25f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is one of your enemies. You can beat them if you're larger, otherwise you", 0.3f, -0.16f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 1.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "die upon contact.", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 6, -5.5f, 0.1f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, 0.1f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is another enemy. These follow you and you cannot beat them.", 0.3f, -0.16f);
    
    Utilities::SheetSprite greenblock(green_block_texture, 1.0f, 1.0f, 2.5f, 0.5f, 0.5f);
    greenblock.Draw(program, model_matrix, 0, -3.0f, -1.3f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -0.8f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "These are basic platforms:", 0.3f, -0.16f);
    
    Utilities::SheetSprite redblock(red_block_texture, 1.0f, 1.0f, 2.5f, 0.5f, 0.5f);
    redblock.Draw(program, model_matrix, 0, 3.0f, -1.3f);
    model_matrix.identity();
    model_matrix.Translate(1.0f, -0.8f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Avoid these, they shrink you:", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 7, -5.5f, -2.6f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -2.6f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Need to quit? Hit ESC.", 0.3f, -0.16f);
    
    model_matrix.identity();
    model_matrix.Translate(-5.1f, -4.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Ready? Press Space or Enter to start.", 0.5f, -0.2f);
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
    if (goal) delete goal;
    
    switch (state)
    {
        case LEVEL1:
            blocks.push_back(new Block(-5.5f, -4.0f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.0f, -3.7f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-1.8f, -2.0f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-2.7f, -0.2f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(0.2f, 0.7f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(3.5f, 1.0f, 0.15f, 0.5f, GREEN));
            for (Block *b : blocks)
            {
                if (b->get_type() == GREEN)
                    b->set_sprite(green_block_texture, 1, 1);
                else if (b->get_type() == RED)
                    b->set_sprite(red_block_texture, 1, 1);
                b->render(program, model_matrix, 0);
            }
            goal = new Coin(6.5f, 1.5f, 0.2f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            if (finished_level)
            {
                finished_level = false;
                startx = -5.5f;
                starty = -2.5f;
                player->set_loc(startx, starty);
                player->reset();
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
    coin_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"coin.png");
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
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        state = TITLE_SCREEN;
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                        if (!player->has_jumped())
                            player->jump();
                }
                if (keys[SDL_SCANCODE_LEFT])
                    player->move(-7.5f);
                else if (keys[SDL_SCANCODE_RIGHT])
                    player->move(7.5f);
                else player->move(0.0f);
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
    if (state == TITLE_SCREEN || state == GAME_OVER)
        return;
    
    player->move_y(elapsed);
    for (Block *b : blocks)
    {
        if (player->is_colliding_on_y_with(b))
        {
            if (b->get_type() == GREEN)
                player->bounce_off_of(b);
            else if (b->get_type() == RED)
                player->update_size(-0.05f);
        }
    }
    player->move_x(elapsed);
    for (Block *b : blocks)
    {
        if (player->is_colliding_on_x_with(b))
        {
            if (b->get_type() == GREEN)
                player->bounce_off_of(b);
            else if (b->get_type() == RED)
                player->update_size(-0.05f);
        }
    }
    
    if (player->get_pos_y() + player->get_height() < -8.0f)
    {
        if (lives > 1)
        {
            lives -= 1;
            player->set_loc(startx, starty);
        }
        else
        {
            state = GAME_OVER;
        }
    }
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
            player->render(program, model_matrix, 0);
            model_matrix.identity();
            model_matrix.Translate(-7.75f, 4.20f, 0.0f);
            program->setModelMatrix(model_matrix);
            std::string lives_string = "Lives: " + std::to_string(lives);
            utilities.DrawText(program, font_texture, lives_string, 0.3f, -0.16f);
        }
        
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
    float fixed_elapsed = elapsed + time_left_over;
    if (fixed_elapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
        fixed_elapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    last_frame_ticks = ticks;
    
    while (fixed_elapsed >= FIXED_TIMESTEP)
    {
        fixed_elapsed -= FIXED_TIMESTEP;
        update(FIXED_TIMESTEP);
    }
    
    time_left_over = fixed_elapsed;
    
    //update(elapsed);
    render();
    return done;
}