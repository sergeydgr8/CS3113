//
//  SpaceInvader.cpp
//  NYUCodebase
//

/*#include "SpaceInvader.h"

SpaceInvader::SpaceInvader() : 
    done(false),
    last_frame_ticks(0.0f),
    attack_interval(1.5f),
    move_interval(3.0f),
    game_won(false),
    game_state(TITLE_SCREEN),
    player(new Entity(0.0f, -7.5f, 1.0f, 2.0f, PLAYER))
    { setup(); }

SpaceInvader::~SpaceInvader()
{
    delete player;
    for (Entity *e : invaders)
    {
        e = nullptr;
        delete e;
    }
    invaders.clear();
    for (Bullet *b : bullets)
    {
        b = nullptr;
        delete b;
    }
    bullets.clear();
}

void SpaceInvader::setup()
{
    SDL_Init(SDL_INIT_VIDEO);
    display_window = SDL_CreateWindow("Assignment 3 - Sergey Smirnov", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);
#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    projection_matrix.setOrthoProjection(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);

    program->setModelMatrix(model_matrix);
    program->setProjectionMatrix(projection_matrix);
    program->setViewMatrix(view_matrix);

    glUseProgram(program->programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    font_tex_id = utilities.LoadRGBATexture("partly_roboto.png");
    bullet_tex_id = utilities.LoadRGBTexture("brightblue.png");
    invaders_tex_id = utilities.LoadRGBATexture("invaders.png");
    player_tex_id = utilities.LoadRGBATexture("ship.png");

    make_invaders(invaders, 20);
}

// assume there are multiples of 5 of invaders
// 5 invaders per row
void SpaceInvader::make_invaders(std::vector<Entity *> &invaders, int num_invaders = 20)
{
    float x = -8.0f;
    float y = 8.0f;

    for (size_t i = 0; i < num_invaders; i++)
    {
        invaders.push_back(new Entity(x, y, 1.0f, 1.0f, INVADER));
        x += 4.0f;

        // new row
        if (i != 0 && (i % 5) == 0)
        {
            x = -8.0f;
            y -= 2.0f;
        }
    }
}

void SpaceInvader::reset()
{
    delete player;
    invaders.clear();
    bullets.clear();

    player = new Entity(0.0f, -7.5f, 1.0f, 2.0f, PLAYER);
    make_invaders(invaders);
    if (game_won) game_won = false;
}

void SpaceInvader::process_events(float elapsed)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            done = true;

        const Uint8 *keys = SDL_GetKeyboardState(nullptr);

        switch (game_state)
        {
        case TITLE_SCREEN:
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN ||
                    event.key.keysym.scancode == SDL_SCANCODE_SPACE ||
                    event.key.keysym.scancode == SDL_SCANCODE_RETURN2)
                    game_state = GAME;
            }
            break;
        case GAME:
            if (keys[SDL_SCANCODE_LEFT])
            {
                if (player->get_pos_x() > -15.0f)
                    player->move_x(-1.0f * elapsed * 50.0f);
            }
            else if (keys[SDL_SCANCODE_RIGHT])
            {
                if (player->get_pos_x() < 15.0f)
                    player->move_x(elapsed * 50.0f);
            }
            else if (keys[SDL_SCANCODE_SPACE])
                player->shoot(bullets);
            break;
        case GAME_OVER:
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN || 
                    event.key.keysym.scancode == SDL_SCANCODE_SPACE ||
                    event.key.keysym.scancode == SDL_SCANCODE_RETURN2)
                {
                    game_state = TITLE_SCREEN;
                    reset();
                }
            }
            break;
        default:
            game_state = TITLE_SCREEN;
        }
    }
}

void SpaceInvader::update(float elapsed)
{
    process_events(elapsed);

    if (game_state == TITLE_SCREEN || game_state == GAME_OVER)
        return;

    attack_interval -= elapsed;
    move_interval -= elapsed;

    if (attack_interval <= 0.0f)
    {
        invader_shoots();
        attack_interval = 1.5f;
    }
    if (move_interval <= 0.0f)
    {
        move_invaders(elapsed);
        move_interval = 3.0f;
    }

    clear_inactive_bullets();
    for (Bullet *b : bullets)
        b->update(elapsed);
    check_for_collisions();
}

void SpaceInvader::invader_shoots()
{
    int rand_invader = rand() % 20;
    while (!invaders[rand_invader]->is_alive()) // checking to see if invader is not alive
        rand_invader = rand() % 20; // get a new invader
    invaders[rand_invader]->shoot(bullets);
}

void SpaceInvader::move_invaders(float elapsed)
{
    for (Entity *invader : invaders)
        if (invader->is_alive())
            invader->move_y(elapsed * 50.0f);
}

bool SpaceInvader::are_all_invaders_dead()
{
    for (Entity *invader : invaders)
        if (invader->is_alive())
            return false;
    return true;
}

bool SpaceInvader::is_colliding_with_bullet(Entity *entity, Bullet *bullet)
{
    float half_bullet = bullet->get_diameter() / 2.0f;
    float bullet_top = bullet->get_pos_y() + half_bullet;
    float bullet_bottom = bullet->get_pos_y() - half_bullet;
    float bullet_left = bullet->get_pos_x() - half_bullet;
    float bullet_right = bullet->get_pos_x() + half_bullet;

    float entity_top = entity->get_pos_y() + (entity->get_height() / 2.0f);
    float entity_bottom = entity->get_pos_y() - (entity->get_height() / 2.0f);
    float entity_left = entity->get_pos_x() - (entity->get_width() / 2.0f);
    float entity_right = entity->get_pos_x() + (entity->get_width() / 2.0f);

    return (bullet_top >= entity_bottom ||
        bullet_bottom <= entity_top ||
        bullet_left <= entity_right ||
        bullet_right >= entity_left);
}

void SpaceInvader::check_for_collisions()
{
    for (Bullet *b : bullets)
    {
        if (b->get_type() == PLAYER)
        {
            for (Entity *i : invaders)
            {
                if (i->is_alive())
                {
                    if (is_colliding_with_bullet(i, b))
                    {
                        i->die();
                        b = nullptr;
                        delete b;
                        //bullets.erase(std::find(bullets.begin(), bullets.end(), b));
                        break;
                    }
                }
            }
        }
        else if (b->get_type() == INVADER)
        {
            if (player->is_alive())
            {
                if (is_colliding_with_bullet(player, b))
                {
                    player->die();
                    b = nullptr;
                    delete b;
                    //bullets.erase(std::find(bullets.begin(), bullets.end(), b));
                    game_state = GAME_OVER;
                    break;
                }
            }
        }
    }
}

void SpaceInvader::clear_inactive_bullets()
{
    for (Bullet *b : bullets) {
        if (b->get_time_left() <= 0.0f)
        {
            b = nullptr;
            delete b;
            //bullets.erase(std::find(bullets.begin(), bullets.end(), b));

        }
    }
}

void SpaceInvader::check_for_win()
{
    if (!player->is_alive())
    {
        game_state = GAME_OVER;
        return;
    }

    for (Entity *e : invaders)
    {
        if ((e->get_pos_y() - (e->get_height() / 2)) <=
            (player->get_pos_y() + (player->get_height() / 2)))
        {
            game_state = GAME_OVER;
            break;
        }
    }

    if (are_all_invaders_dead())
    {
        game_state = GAME_OVER;
        game_won = true;
    }
}

void SpaceInvader::render_title_screen()
{
    model_matrix.identity();
    model_matrix.Translate(-9.0f, 5.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_tex_id, "Space Invaders!", 3.0f, -1.6f);

    model_matrix.identity();
    model_matrix.Translate(-10.5f, 3.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_tex_id, "NYU-Poly CS-UY 3113 Project - Sergey Smirnov", 1.0f, -0.5f);

    model_matrix.identity();
    model_matrix.Translate(-6.0f, -3.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_tex_id, "Press Enter or Space to start.", 1.0f, -0.55f);
}

void SpaceInvader::render_game()
{
    player->draw(player_tex_id, program, model_matrix);
    for (Entity *invader : invaders)
        if (invader->is_alive())
            utilities.DrawSpriteSheetSprite(program, model_matrix, 0, 4, 2, invaders_tex_id, invader->get_pos_x(), invader->get_pos_y());

    for (Bullet *bullet : bullets)
        bullet->draw(bullet_tex_id, program, model_matrix);
}

void SpaceInvader::render_game_over()
{
    model_matrix.identity();
    model_matrix.Translate(-10.0f, 0.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    if (game_won) utilities.DrawText(program, font_tex_id, "You win!", 3.0f, -1.5f);
    else utilities.DrawText(program, font_tex_id, "You lose :(", 4.0f, 0.0f);
    utilities.DrawText(program, font_tex_id, "Press Enter or Space to continue.", 1.0f, 0.0f);
}

void SpaceInvader::render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.14f, 0.215f, 0.309f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch (game_state)
    {
    case TITLE_SCREEN:
        render_title_screen();
        break;
    case GAME:
        render_game();
        break;
    case GAME_OVER:
        render_game_over();
        break;
    default:
        render_title_screen();
    }
    SDL_GL_SwapWindow(display_window);
}

bool SpaceInvader::update_and_render()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - last_frame_ticks;
    last_frame_ticks = ticks;

    update(elapsed);
    render();
    return done;
}*/