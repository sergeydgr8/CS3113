//
//  Platformer.cpp
//  NYUCodebase
//

#include "Platformer.hpp"

Platformer::Platformer()
{
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

// TODO: Build all the levels
void Platformer::build_map()
{
    switch (state)
    {
        case LEVEL1:
            // render level 1
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

void Platformer::render_map()
{
    std::vector<float> vertex_data;
    std::vector<float> tex_coord_data;
    
    int SPRITE_COUNT_X = 18;
    int SPRITE_COUNT_Y = 1;
    
    int counter = 0;
    
    for (size_t y = 0; y < map_height; y++)
    {
        for (size_t x = 0; x < map_width; x++)
        {
            if ((int) level_data[y][x] != 0)
            {
                float u = (float) (((int) level_data[y][x] - 1) %
                                   SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float) (((int) level_data[y][x] - 1) /
                                   SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            
                float sprite_width = 1.0f / (float) SPRITE_COUNT_X;
                float sprite_height = 1.0f / (float) SPRITE_COUNT_Y;
            
                vertex_data.insert(vertex_data.end(),
                {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
                
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
                });
            
                tex_coord_data.insert(tex_coord_data.end(),
                {
                    u, v,
                    u, v + sprite_height,
                    u + sprite_width, v + sprite_height,
                
                    u, v,
                    u + sprite_width, v + sprite_height,
                    u + sprite_width, v
                });
                counter++;
            }
        }
    }
    
    try
    {
        glUseProgram(program->programID);
        model_matrix.identity();
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertex_data.data());
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coord_data.data());
        
        glEnableVertexAttribArray(program->positionAttribute);
        glEnableVertexAttribArray(program->texCoordAttribute);
        
        program->setModelMatrix(model_matrix);
        glBindTexture(GL_TEXTURE_2D, block_texture);
        //glDrawArrays(GL_TRIANGLES, 0, 6 * 32 * 128);
        glDrawArrays(GL_TRIANGLES, 0, 6 * counter);
        
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in drawing: " << e.what() << "\n";
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
    green_block_texture = utilities.LoadRGBTexture(RESOURCE_FOLDER"green_block.png");
    red_block_texture = utilities.LoadRGBTexture(RESOURCE_FOLDER"red_block.png");
    font_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"partly_roboto.png");
}

void Platformer::process_events(float elapsed)
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
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    // jump!
                    player->jump();
                }
            }
            if (keys[SDL_SCANCODE_LEFT])
            {
                // go left!
                player->move(-20.0f);
            }
            else if (keys[SDL_SCANCODE_RIGHT])
            {
                // go right!
                player->move(20.0f);
            }
            else player->move(0.0f);
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in process_events(): " << e.what() << "\n";
    }
    
}

void Platformer::update(float elapsed)
{
    process_events(elapsed);
    player->update(elapsed);
    check_for_collisions();
    scroll_screen();
}

void Platformer::render()
{
    try
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program->programID);
        
        
        program->setProjectionMatrix(projection_matrix);
        program->setViewMatrix(view_matrix);
        
        render_map();
        player->render(program, model_matrix, 0);
        
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