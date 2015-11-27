//
//  SpaceInvader.cpp
//  NYUCodebase
//

#include "Platformer.h"

Platformer::Platformer() : done(false)
{
    setup();
    create_player();
    build_level();
}

Platformer::~Platformer() {}

void Platformer::setup()
{
    SDL_Init(SDL_INIT_VIDEO);
    display_window = SDL_CreateWindow("Assignment 4 - Sergey Smirnov", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);
#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);
    projection_matrix.setOrthoProjection(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);

    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

    program->setModelMatrix(model_matrix);
    program->setProjectionMatrix(projection_matrix);
    program->setViewMatrix(view_matrix);

    glUseProgram(program->programID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Platformer::process_events()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            done = true;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        if (keys[SDL_SCANCODE_LEFT])
            player->move_x((-1) * player_velocity);
        else if (keys[SDL_SCANCODE_RIGHT])
            player->move_x(player_velocity);
        else player->move_x(0);

        if (keys[SDL_SCANCODE_UP])
            if (player->is_standing()) player->move_y(player_velocity);
    }
}

void Platformer::update(float elapsed)
{
    process_events();
    player->Update(elapsed);
    for (Entity *block : blocks)
        player->collides_with(block);
    scroll_screen();
}


void Platformer::render()
{
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glClearColor(0.14f, 0.215f, 0.309f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    render_blocks();
    player->Render(program, model_matrix);
    SDL_GL_SwapWindow(display_window);
}

bool Platformer::update_and_render()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - last_frame_ticks;
    last_frame_ticks = ticks;

    update(elapsed);
    render();
    return done;
}

void Platformer::create_player()
{
    player_texture_id = utilities.LoadRGBATexture("george_0.png");
    player = new Entity(0.0f, 0.0f, (float)(2.9f * BLOCK_SIZE), (float)(3.1f * BLOCK_SIZE), false, ENTITY_PLAYER);
    player->set_sprite(player_texture_id, 10.0f / 192.0f, 11.0f / 192.0f, 29.0f / 192.0f, 31.0f / 192.f, 0.2f);
}

void Platformer::build_level()
{
    blocks_texture_id = utilities.LoadRGBTexture("JnRTiles.png");
    std::ifstream platform("map.txt");
    std::string line;

    while (getline(platform, line))
    {
        if (line == "[header]")
        {
            if (!read_header(platform))
                return;
        }
        else if (line == "[layer]")
            read_layer_data(platform);
        
    }

    platform.close();

    for (size_t i = 0; i < map_height; i++)
    {
        for (size_t j = 0; j < map_width; j++)
        {
            if (level_data[i][j] == '1')
            {
                blocks.push_back(new Entity(
                    (BLOCK_SIZE * j) + (BLOCK_SIZE / 2.0f),
                    (-BLOCK_SIZE * i) - (BLOCK_SIZE / 2.0f),
                    BLOCK_SIZE,
                    BLOCK_SIZE,
                    true,
                    ENTITY_BLOCK));
            }
        }
    }
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
            for (size_t i = 0; i < map_height; i++)
            {
                getline(stream, line);
                std::istringstream line_stream(line);
                std::string tile;

                for (size_t j = 0; j < map_width; j++)
                {
                    getline(line_stream, tile, ',');
                    unsigned char val = (unsigned char)atoi(tile.c_str());
                    if (val > 0)
                        level_data[i][j] = val - 1;
                    else
                        level_data[i][j] = 0;
                }
            }
        }
    }
    return true;
}

void Platformer::render_blocks()
{
    std::vector<float> vertex_data;
    std::vector<float> tex_coord_data;

    int sprite_x = 18;
    int sprite_y = 1;

    for (size_t y = 0; y < map_height; y++)
    {
        for (size_t x = 0; x < map_width; x++)
        {
            float u = (float)(((int)level_data[y][x] % sprite_x) / (float)sprite_x);
            float v = (float)(((int)level_data[y][x] / sprite_x) / (float)sprite_x);

            float sprite_width = 1.0f / (float)sprite_x;
            float sprite_height = 1.0f / (float)sprite_y;

            vertex_data.insert(vertex_data.end(), {
                BLOCK_SIZE * x, -BLOCK_SIZE * y,
                BLOCK_SIZE * x, (-BLOCK_SIZE * y) - BLOCK_SIZE,
                (BLOCK_SIZE * x) + BLOCK_SIZE, (-BLOCK_SIZE * y) - BLOCK_SIZE,
                BLOCK_SIZE * x, -BLOCK_SIZE * y,
                (BLOCK_SIZE * x) + BLOCK_SIZE, (-BLOCK_SIZE * y) - BLOCK_SIZE,
                (BLOCK_SIZE * x) + BLOCK_SIZE, -BLOCK_SIZE * y
            });
            tex_coord_data.insert(tex_coord_data.end(), {
                u, v,
                u, v + sprite_height,
                u + sprite_width, v + sprite_height,
                u, v,
                u + sprite_width, v + sprite_height,
                u + sprite_width, v
            });
        }
    }

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertex_data.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coord_data.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    model_matrix.identity();
    program->setModelMatrix(model_matrix);

    glBindTexture(GL_TEXTURE_2D, blocks_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 128 * 32);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Platformer::scroll_screen()
{
    view_matrix.identity();
    view_matrix.Translate(-player->get_x(), -player->get_y(), 0.0f);
    program->setViewMatrix(view_matrix);
}