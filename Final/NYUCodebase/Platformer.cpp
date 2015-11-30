//
//  Platformer.cpp
//  NYUCodebase
//

#include "Platformer.hpp"

Platformer::Platformer()
{
    setup();
    player = new Entity(1.0f * TILE_SIZE, 1.0f * TILE_SIZE, 0.0f, -15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.81f, PLAYER);
    character_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"george_0.png");
    player->set_sprite(character_texture, 4.0f, 4.0f, player->get_width(), player->get_height(), 1.0f);
    build_map();
}

Platformer::~Platformer()
{
    delete program;
    SDL_Quit();
}

bool Platformer::read_header(std::ifstream &stream)
{
    std::string line;
    map_width = -1;
    map_height = -1;
    while (getline(stream, line))
    {
        if (line == "") break;
        
        std::istringstream sStream(line);
        std::string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if (key == "width")
            map_width = atoi(value.c_str());
        else if (key == "height")
            map_height = atoi(value.c_str());
    }
    
    if (map_width == -1 || map_height == -1)
        return false;
    else
    {
        level_data = new unsigned char*[map_height];
        for (size_t i = 0; i < map_height; i++)
            level_data[i] = new unsigned char[map_width];
        return true;
    }
}

bool Platformer::read_layer_data(std::ifstream &stream)
{
    std::string line;
    while (getline(stream, line))
    {
        if (line == "") break;
        
        std::istringstream sStream(line);
        std::string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if (key == "data")
        {
            for (size_t y = 0; y < map_height; y++)
            {
                getline(stream, line);
                std::istringstream lineStream(line);
                std::string tile;
                
                for (size_t x = 0; x < map_width; x++)
                {
                    getline(lineStream, tile, ',');
                    unsigned char val = (unsigned char) atoi(tile.c_str());
                    if (val > 0)
                        level_data[y][x] = val;
                        //level_data[y][x] = val - 1; // careful, tiles start from 1, not 0
                    else level_data[y][x] = 0;
                }
            }
        }
    }
    return true;
}

void Platformer::build_map()
{
    block_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"JnRTiles.png");
    std::ifstream file_stream(RESOURCE_FOLDER"map.txt");
    std::string line;
    while (getline(file_stream, line))
    {
        if (line == "[header]")
        {
            if (!read_header(file_stream)) return;
        }
        else if (line == "[layer]")
            read_layer_data(file_stream);
    }
    file_stream.close();
    
    for (size_t y = 0; y < map_height; y++)
        for (size_t x = 0; x < map_width; x++)
            if ((int)level_data[y][x] == 1)
                blocks.push_back(new Block((TILE_SIZE * x) + (TILE_SIZE / 2.0f),
                                           (-TILE_SIZE * y) - (TILE_SIZE / 2.0f),
                                           TILE_SIZE));
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

void Platformer::scroll_screen()
{
    view_matrix.identity();
    view_matrix.Translate(-(player->get_pos_x()), -(player->get_pos_y()), 0.0f);
    program->setViewMatrix(view_matrix);
}

void Platformer::setup()
{
    try
    {
        SDL_Init(SDL_INIT_VIDEO);
        display_window = SDL_CreateWindow("Assignment 4 - Sergey Smirnov", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(display_window);
        SDL_GL_MakeCurrent(display_window, context);
#ifdef _WINDOWS
        glewInit();
#endif
        
        glViewport(0, 0, 1280, 720);
        
        program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
        
        projection_matrix.setOrthoProjection(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
        glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
        
        view_matrix.identity();
        view_matrix.Translate(0.0f, -5.0f, 0.0f);
        
        //glEnable(GL_NEAREST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        done = false;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception in setup(): " << e.what() << "\n";
    }
    
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