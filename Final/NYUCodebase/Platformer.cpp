//
//  Platformer.cpp
//  NYUCodebase
//

#include "Platformer.hpp"

Platformer::Platformer()
{
    //cheatmode = true; // uncomment this to jump to a certain level from title screen
    state = TITLE_SCREEN;
    window_setup();
    texture_setup();
    startx = -5.5f;
    starty = -3.0f;
    player = new Entity(startx, starty, 0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f);
    player->set_sprite(emojis_texture, 8.0f, 1.0f);
    lives = 5;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    hit = Mix_LoadWAV(RESOURCE_FOLDER"hit.wav");
    kill = Mix_LoadWAV(RESOURCE_FOLDER"kill.wav");
    die = Mix_LoadWAV(RESOURCE_FOLDER"die.wav");
    jump = Mix_LoadWAV(RESOURCE_FOLDER"jump.wav");
    coin = Mix_LoadWAV(RESOURCE_FOLDER"coin.wav");
    music = Mix_LoadMUS(RESOURCE_FOLDER"music.wav");
    Mix_PlayMusic(music, -1);
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
    Mix_FreeChunk(hit);
    Mix_FreeChunk(kill);
    Mix_FreeChunk(die);
    Mix_FreeChunk(jump);
    Mix_FreeChunk(coin);
    Mix_FreeMusic(music);
    SDL_Quit();
}

void Platformer::render_title_screen()
{
    finished_level = true;
    lives = 5;
    win = false;
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
    Utilities::SheetSprite coin(coin_texture, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f);
    Utilities::SheetSprite growblock(growblock_texture, 1.0f, 1.0f, 1.0f, 1.0f, 0.3f);
    
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
    greenblock.Draw(program, model_matrix, 0, -3.0f, -1.15f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -0.65f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "These are basic platforms:", 0.3f, -0.16f);
    
    Utilities::SheetSprite redblock(red_block_texture, 1.0f, 1.0f, 2.5f, 0.5f, 0.5f);
    redblock.Draw(program, model_matrix, 0, 3.0f, -1.15f);
    model_matrix.identity();
    model_matrix.Translate(1.0f, -0.65f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Avoid these, they shrink you:", 0.3f, -0.16f);
    
    coin.Draw(program, model_matrix, 0, -5.5, -2.0f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -1.85f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "This is your goal. You can only", 0.3f, -0.16f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -2.15f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "get it if you're bigger than it.", 0.3f, -0.16f);
    
    growblock.Draw(program, model_matrix, 0, 1.25f, -2.0f);
    model_matrix.identity();
    model_matrix.Translate(1.75f, -2.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Eat these to grow.", 0.3f, -0.16f);
    
    emojis.Draw(program, model_matrix, 7, -5.5f, -3.0f);
    model_matrix.identity();
    model_matrix.Translate(-4.75f, -3.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Need to quit? Hit ESC.", 0.3f, -0.16f);
    
    model_matrix.identity();
    model_matrix.Translate(-5.1f, -4.0f, 0.0f);
    program->setModelMatrix(model_matrix);
    utilities.DrawText(program, font_texture, "Ready? Press Space or Enter to start.", 0.5f, -0.2f);
}

void Platformer::render_game_over_screen()
{
    if (win)
    {
        model_matrix.identity();
        model_matrix.Translate(-3.5, 0.0f, 0.0f);
        program->setModelMatrix(model_matrix);
        utilities.DrawText(program, font_texture, "YOU WIN!", 2.0f, -1.0f);
        
        model_matrix.identity();
        model_matrix.Translate(-5.8f, -4.0f, 0.0f);
        program->setModelMatrix(model_matrix);
        utilities.DrawText(program, font_texture, "Press space, enter, or escape to go back to main menu.", 0.5f, -0.27f);
    }
    else
    {
        model_matrix.identity();
        model_matrix.Translate(-3.75f, 0.0f, 0.0f);
        program->setModelMatrix(model_matrix);
        utilities.DrawText(program, font_texture, "You lose.", 2.0f, -1.0f);
        
        model_matrix.identity();
        model_matrix.Translate(-5.8f, -4.0f, 0.0f);
        program->setModelMatrix(model_matrix);
        utilities.DrawText(program, font_texture, "Press space, enter, or escape to go back to main menu.", 0.5f, -0.27f);
    }
}

// TODO: Build all the levels
void Platformer::build_map()
{
    for (size_t i = 0; i < blocks.size(); i++)
        delete blocks[i];
    blocks.clear();
    
    switch (state)
    {
        case LEVEL1:
            blocks.push_back(new Block(-5.5f, -4.0f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.0f, -3.7f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-1.8f, -2.0f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-2.7f, -0.2f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(0.2f, 0.7f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(3.5f, 1.0f, 0.15f, 0.5f, GREEN));

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
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
            }
            break;
        case LEVEL2:
            blocks.push_back(new Block(-3.5f, -4.2f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.5f, -3.0f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.5f, -1.8f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.5f, -0.6f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.5f, 0.6f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-3.5f, 1.8f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-5.25f, -3.0f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-5.25f, -1.8f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-5.25f, -0.6f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-5.25f, 0.6f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-5.25f, 1.8f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-1.75f, -3.0f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-1.75f, -1.8f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-1.75f, -0.6f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-1.75f, 0.6f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-1.75f, 1.8f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(1.5f, -3.0f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(3.5f, -4.2f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(4.5f, -3.0f, 0.15f, 0.5f, GREEN));

            goal = new Coin(3.5f, -0.75f, 0.2f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -3.5f;
                starty = -3.6f;
                player->set_loc(startx, starty);
                player->reset();
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
            }
            break;
        case LEVEL3:
            blocks.push_back(new Block(-1.5f, 2.0f, 0.3f, 0.75f, GREEN));
            blocks.push_back(new Block(0.0f, 0.3f, 0.15f, 0.3f, RED));
            blocks.push_back(new Block(-1.6f, -0.6f, 0.15f, 1.9f, GREEN));
            blocks.push_back(new Block(-2.5f, 0.3f, 0.15f, 0.3f, RED));
            blocks.push_back(new Block(2.0f, 0.3f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(2.0f, -1.1f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(4.0f, -1.8f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(6.0f, -1.1f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-6.0f, -0.6f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-3.8f, -0.9f, 0.15f, 0.3f, RED));
            blocks.push_back(new Block(-4.4f, -1.5f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-4.9f, -3.2f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-3.0f, -4.2f, 0.15f, 0.3f, GREEN));
            
            goal = new Coin(-1.5f, 4.0f, 0.4f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);

            if (finished_level)
            {
                finished_level = false;
                startx = -1.5f;
                starty = 2.9f;
                player->set_loc(startx, starty);
                player->reset();
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(-0.65f, 1.8f));
                growblocks.push_back(new Growblock(1.8f, -0.3f));
                growblocks.push_back(new Growblock(5.8f, -0.3f));
                growblocks.push_back(new Growblock(-4.9f, -2.0f));
                growblocks.push_back(new Growblock(-3.3f, -3.3f));
            }
            break;
        case LEVEL4:
            blocks.push_back(new Block(-1.5f, 1.75f, 0.4f, 0.75f, GREEN));
            blocks.push_back(new Block(0.0f, 3.5f, 0.15f, 0.9f, RED));
            blocks.push_back(new Block(1.0f, -2.5f, 0.15f, 1.5f, GREEN));
            blocks.push_back(new Block(4.0f, -1.8f, 0.15f, 1.5f, GREEN));
            blocks.push_back(new Block(-3.0f, -0.7f, 0.15f, 0.5f, GREEN));
            blocks.push_back(new Block(-5.0f, 1.0f, 0.15f, 0.5f, GREEN));
            
            goal = new Coin(-1.5f, 4.0f, 0.5f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -1.5f;
                starty = 2.9f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(1.0f, -1.4f, 0.35f, 0.35f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(4.0f, -0.8f, 0.55f, 0.55f, DEFINED_PATH, 2.0f, 0.0f));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(-0.35f, -1.5f));
                growblocks.push_back(new Growblock(0.25f, -1.5f));
                growblocks.push_back(new Growblock(0.85f, -1.5f));
                growblocks.push_back(new Growblock(1.45f, -1.5f));
                growblocks.push_back(new Growblock(2.05f, -1.5f));
                growblocks.push_back(new Growblock(2.60f, -0.8f));
                growblocks.push_back(new Growblock(3.20f, -0.8f));
                growblocks.push_back(new Growblock(3.80f, -0.8f));
                growblocks.push_back(new Growblock(4.40f, -0.8f));
                growblocks.push_back(new Growblock(5.0f, -0.8f));
            }
            break;
        case LEVEL5:
            blocks.push_back(new Block(-5.0f, 1.75f, 0.4f, 0.75f, GREEN));
            blocks.push_back(new Block(-2.0f, -3.5f, 0.15f, 1.5f, RED));
            blocks.push_back(new Block(-1.5f, -3.2f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(1.25f, -1.5f, 0.15f, 1.5f, GREEN));
            blocks.push_back(new Block(4.0f, -2.0f, 2.0f, 0.05f, GREEN));
            blocks.push_back(new Block(5.0f, -2.0f, 2.0f, 0.05f, GREEN));
            blocks.push_back(new Block(4.5f, -1.0f, 1.2f, 0.4f, RED));
            
            goal = new Coin(4.5f, -3.8f, 0.05f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -5.0f;
                starty = 2.9f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(-2.0f, -1.5f, 0.35f, 0.35f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(1.25f, -0.5f, 0.5f, 0.5f, DEFINED_PATH, 2.0f, 0.0f));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(-3.25f, -1.5f));
                growblocks.push_back(new Growblock(-2.65f, -1.5f));
                growblocks.push_back(new Growblock(-2.05f, -1.5f));
                growblocks.push_back(new Growblock(-1.45f, -1.5f));
                growblocks.push_back(new Growblock(-0.85f, -1.5f));
                growblocks.push_back(new Growblock(-0.15f, -0.5f));
                growblocks.push_back(new Growblock(0.55f, -0.5f));
                growblocks.push_back(new Growblock(1.25f, -0.5f));
                growblocks.push_back(new Growblock(1.95f, -0.5f));
                growblocks.push_back(new Growblock(2.65f, -0.5f));
            }
            break;
        case LEVEL6:
            blocks.push_back(new Block(-6.6f, 4.5f, 2.0f, 0.05f, GREEN));
            blocks.push_back(new Block(-4.55f, 2.65f, 0.15f, 2.0f, GREEN));
            blocks.push_back(new Block(-2.05f, 2.65f, 0.15f, 0.5f, RED));
            blocks.push_back(new Block(-2.55f, 1.0f, 0.15f, 1.0f, GREEN));
            blocks.push_back(new Block(-3.25f, 1.8f, 0.6f, 0.3f, RED));
            blocks.push_back(new Block(4.0f, -4.0f, 0.15f, 2.0f, GREEN));
            blocks.push_back(new Block(0.5f, -4.0f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-2.8f, -4.0f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-4.85f, -4.0f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-4.85f, -2.75f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-4.85f, -1.5f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-4.85f, -0.25f, 0.15f, 0.35f, GREEN));
            blocks.push_back(new Block(-4.20f, -0.5f, 1.5f, 0.3f, RED));
            blocks.push_back(new Block(-6.15f, -0.5f, 1.5f, 0.3f, RED));
            
            goal = new Coin(-4.85f, 1.5f, 0.15f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -4.55f;
                starty = 3.1f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(-7.0f, -3.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(0.0f, -1.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-3.25f, 3.0f, 0.2f, 0.2f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(-1.5f, 3.0f, 0.2f, 0.2f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(2.0f, 0.0f, 0.5f, 0.5f, DEFINED_PATH, 2.0f, 0.0f));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(-2.7f, 3.1f));
                growblocks.push_back(new Growblock(-4.85f, 0.25f));
            }
            break;
        case LEVEL7:
            blocks.push_back(new Block(0.0f, -4.35f, 0.15f, 2.5f, GREEN));
            blocks.push_back(new Block(1.5f, -4.05f, 0.15f, 0.3f, RED));
            blocks.push_back(new Block(2.1f, -3.0f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(1.75f, -2.85f, 0.3f, 0.05f, GREEN));
            blocks.push_back(new Block(1.25f, -1.65f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(4.4f, -0.25f, 0.15f, 1.5f, GREEN));
            blocks.push_back(new Block(7.25f, 0.75f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(3.2f, 0.75f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(1.75f, 1.75f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-1.0f, 2.25f, 0.15f, 1.5f, GREEN));
            blocks.push_back(new Block(0.75f, 3.25f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-1.75f, 1.0f, 0.15f, 0.75f, RED));
            blocks.push_back(new Block(-2.75f, -0.5f, 0.15f, 0.3f, GREEN));
            
            goal = new Coin(2.75f, 3.70f, 0.275f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = 0.0f;
                starty = -3.8f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(1.5f, -4.0f, 0.2f, 0.2f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(-1.75f, 1.35f, 0.2f, 0.2f, DEFINED_PATH, 2.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-4.5f, 3.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-2.25, 2.25f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-2.8f, -1.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-6.5f, -3.85f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(-1.8f, -3.5f));
                growblocks.push_back(new Growblock(-2.35f, 3.0f));
                growblocks.push_back(new Growblock(-2.35f, 3.50f));
            }
            break;
        case LEVEL8:
            blocks.push_back(new Block(-4.0f, -2.75f, 0.15f, 3.5f, GREEN));
            blocks.push_back(new Block(-1.5f, -2.45f, 0.15f, 1.0f, RED));
            blocks.push_back(new Block(-2.75f, -1.25f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-0.8f, -1.05f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(1.5f, -3.25f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(4.5f, -2.45f, 0.15f, 1.75f, GREEN));
            blocks.push_back(new Block(3.05f, -2.15f, 0.15f, 0.3f, RED));
            blocks.push_back(new Block(7.0f, -1.5f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(6.25f, 0.35f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(2.25f, 0.25f, 0.15f, 1.75f, GREEN));
            blocks.push_back(new Block(0.5f, 1.5f, 0.15f, 0.3f, GREEN));
            
            goal = new Coin(-2.75f, 1.6f, 0.2f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -3.0f;
                starty = -2.15f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(-3.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-1.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(1.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(3.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(5.0f, 4.0f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, 4.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, 2.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, 0.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, -1.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(-7.0f, -2.5f, 0.25f, 0.25f, FOLLOWING, 0.0f, 0.0f));
                enemies.push_back(new Enemy(6.0f, -2.2f, 0.2f, 0.2f, DEFINED_PATH, 2.0f, 0.0f));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                growblocks.push_back(new Growblock(6.0f, 1.0f));
            }
            break;
        case LEVEL9:
            blocks.push_back(new Block(0.0f, -3.0f, 0.15f, 7.0f, GREEN));
            blocks.push_back(new Block(1.5f, -2.7f, 0.15f, 3.0f, RED));
            blocks.push_back(new Block(1.0f, -0.5f, 0.15f, 4.5f, GREEN));
            blocks.push_back(new Block(1.0f, -0.8f, 0.15f, 2.5f, RED));
            blocks.push_back(new Block(7.25f, -2.0f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(6.15f, -1.0f, 0.15f, 0.3f, GREEN));
            blocks.push_back(new Block(-3.9f, 0.4f, 0.75f, 0.4f, GREEN));
            blocks.push_back(new Block(-6.3f, 1.3f, 0.15f, 2.0f, GREEN));
            blocks.push_back(new Block(-3.5f, 3.0f, 0.15f, 0.3f, GREEN));
            
            goal = new Coin(-2.0f, 3.0f, 0.2f);
            goal->set_sprite(coin_texture, 1, 1);
            goal->render(program, model_matrix, 0);
            
            if (finished_level)
            {
                finished_level = false;
                startx = -4.5f;
                starty = -2.5f;
                player->set_loc(startx, starty);
                player->reset();
                player->update_size(-0.1f);
                
                for (size_t i = 0; i < enemies.size(); i++)
                    delete enemies[i];
                enemies.clear();
                enemies.push_back(new Enemy(5.75f, -2.5f, 0.25f, 0.25f, DEFINED_PATH, 2.0f, 0.0f, ENEMY1));
                enemies.push_back(new Enemy(4.5f, -2.5f, 0.25f, 0.25f, DEFINED_PATH, 2.0f, 0.0f, ENEMY1));
                enemies.push_back(new Enemy(7.0f, -2.5f, 0.25f, 0.25f, DEFINED_PATH, 2.0f, 0.0f, ENEMY1));
                enemies.push_back(new Enemy(3.0f, 0.0f, 0.35f, 0.35f, DEFINED_PATH, 2.0f, 0.0f, ENEMY2));
                enemies.push_back(new Enemy(1.5f, 0.0f, 0.35f, 0.35f, DEFINED_PATH, 2.0f, 0.0f, ENEMY2));
                enemies.push_back(new Enemy(-1.0f, 0.0f, 0.35f, 0.35f, DEFINED_PATH, -2.0f, 0.0f, ENEMY2));
                enemies.push_back(new Enemy(0.5f, 0.0f, 0.35f, 0.35f, DEFINED_PATH, -2.0f, 0.0f, ENEMY2));
                
                for (size_t i = 0; i < growblocks.size(); i++)
                    delete growblocks[i];
                growblocks.clear();
                //growblocks.push_back(new Growblock(-4.0f, -2.0f));
                //growblocks.push_back(new Growblock(-3.8f, -2.0f));
                //growblocks.push_back(new Growblock(-3.6f, -2.0f));
                growblocks.push_back(new Growblock(-3.4f, -2.0f));
                growblocks.push_back(new Growblock(-3.2f, -2.0f));
                growblocks.push_back(new Growblock(-3.0f, -2.0f));
                growblocks.push_back(new Growblock(-2.8f, -2.0f));
                growblocks.push_back(new Growblock(6.5f, 0.5f));
            }
            break;
        case LEVEL10:
            // render level 10
            break;
        default:
            break;
    }
    for (Block *b : blocks)
    {
        if (b->get_type() == GREEN)
            b->set_sprite(green_block_texture, 1, 1);
        else if (b->get_type() == RED)
            b->set_sprite(red_block_texture, 1, 1);
        b->render(program, model_matrix, 0);
    }
    for (Growblock *g : growblocks)
    {
        g->set_sprite(growblock_texture, 1, 1);
        g->render(program, model_matrix, 0);
    }
    for (Enemy *e : enemies)
    {
        if (e->is_alive())
        {
            e->set_sprite(emojis_texture, 8, 1);
            if (e->get_type() == DEFINED_PATH)
                e->render(program, model_matrix, 4);
            else if (e->get_type() == FOLLOWING)
                e->render(program, model_matrix, 6);
        }
    }
}

bool Platformer::growblock_is_collided(Growblock *g)
{
    return player->is_colliding_with(g);
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
    growblock_texture = utilities.LoadRGBATexture(RESOURCE_FOLDER"growblock.png");
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
                        if (event.key.keysym.scancode == SDL_SCANCODE_G) {
                            //win = true;
                            state = GAME_OVER;
                        }
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
                        {
                            player->jump();
                            Mix_PlayChannel(-1, jump, 0);
                        }
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
                        event.key.keysym.scancode == SDL_SCANCODE_SPACE ||
                        event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
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
            {
                player->bounce_off_of(b);
            }
            else if (b->get_type() == RED)
            {
                player->update_size(-0.001f);
                player->render(program, model_matrix, 0);
                player->reset_collisions();
            }
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
            {
                player->update_size(-0.001f);
                player->render(program, model_matrix, 0);
                player->reset_collisions();
            }
        }
    }
    
    for (size_t i = 0; i < growblocks.size(); i++)
    {
        if (player->is_colliding_with(growblocks[i]))
        {
            player->update_size(0.05f);
            growblocks[i]->hit();
            growblocks[i]->render(program, model_matrix, 0);
            Mix_PlayChannel(-1, hit, 0);
        }
    }
    
    for (Enemy *e : enemies)
    {
        if (e->is_alive())
        {
            if (e->get_type() == DEFINED_PATH)
            {
                switch (state)
                {
                    case LEVEL4:
                        if (fabs(e->get_original_x() - e->get_pos_x()) > 1.0f)
                            e->switch_direction();
                        break;
                    case LEVEL5:
                        if (fabs(e->get_original_x() - e->get_pos_x()) > 1.0f)
                            e->switch_direction();
                        break;
                    case LEVEL6:
                        if (fabs(e->get_original_x() - e->get_pos_x()) > 2.5f)
                            e->switch_direction();
                        break;
                    case LEVEL7:
                        if (fabs(e->get_original_x() - e->get_pos_x()) > 2.5f)
                            e->switch_direction();
                        break;
                    case LEVEL8:
                        if (fabs(e->get_original_x() - e->get_pos_x()) > 3.25f)
                            e->switch_direction();
                        break;
                    case LEVEL9:
                        if (e->get_channel() == ENEMY1)
                        {
                            if (fabs(e->get_original_x() - e->get_pos_x()) > 1.25f)
                                e->switch_direction();
                        }
                        else if (e->get_channel() == ENEMY2)
                        {
                            if (fabs(e->get_original_x() - e->get_pos_x()) > 2.5f)
                                e->switch_direction();
                        }
                    default:
                        break;
                }
                e->move_x(elapsed);
                
                if (player->is_colliding_with(e))
                {
                    if (player->get_height() >= e->get_height())
                    {
                        e->die();
                        Mix_PlayChannel(-1, kill, 0);
                    }
                    else
                    {
                        if (lives > 1)
                        {
                            lives -= 1;
                            player->reset();
                            player->set_loc(startx, starty);
                            finished_level = true;
                        }
                        else
                        {
                            lives = 6;
                            state = GAME_OVER;
                        }
                        Mix_PlayChannel(-1, die, 0);
                    }
                }
            }
            else if (e->get_type() == FOLLOWING)
            {
                e->move_towards(player, elapsed);
                if (player->is_colliding_with(e))
                {
                    if (lives > 1)
                    {
                        lives -= 1;
                        player->reset();
                        player->set_loc(startx, starty);
                        finished_level = true;
                    }
                    else
                    {
                        lives = 6;
                        state = GAME_OVER;
                    }
                    Mix_PlayChannel(-1, die, 0);
                }
            }
        }
    }

    if (player->is_colliding_on_y_with(goal) || player->is_colliding_on_x_with(goal))
    {
        if (player->get_height() >= goal->get_height())
        {
            Mix_PlayChannel(-1, coin, 0);
            switch (state)
            {
                case LEVEL1:
                    finished_level = true;
                    state = LEVEL2;
                    break;
                case LEVEL2:
                    finished_level = true;
                    state = LEVEL3;
                    break;
                case LEVEL3:
                    finished_level = true;
                    state = LEVEL4;
                    break;
                case LEVEL4:
                    finished_level = true;
                    state = LEVEL5;
                    break;
                case LEVEL5:
                    finished_level = true;
                    state = LEVEL6;
                    break;
                case LEVEL6:
                    finished_level = true;
                    state = LEVEL7;
                    break;
                case LEVEL7:
                    finished_level = true;
                    state = LEVEL8;
                    break;
                case LEVEL8:
                    finished_level = true;
                    state = LEVEL9;
                    break;
                case LEVEL9:
                    finished_level = true;
                    //state = LEVEL10;
                    win = true;
                    state = GAME_OVER;
                    break;
                case LEVEL10:
                    finished_level = true;
                    win = true;
                    state = GAME_OVER;
                    break;
                default:
                    break;
            }
        }
        else
        {
            if (lives > 1)
            {
                lives -= 1;
                player->reset();
                player->set_loc(startx, starty);
                finished_level = true;
            }
            else
            {
                lives = 6;
                state = GAME_OVER;
            }
            Mix_PlayChannel(-1, die, 0);
        }
    }
    
    if (player->get_pos_y() + player->get_height() < -8.0f)
    {
        if (lives > 1)
        {
            lives -= 1;
            player->reset();
            player->set_loc(startx, starty);
            finished_level = true;
        }
        else
        {
            lives = 6;
            state = GAME_OVER;
        }
        Mix_PlayChannel(-1, die, 0);
    }
    
    if (player->get_height() <= 0 || player->get_width() <= 0)
    {
        if (lives > 1)
        {
            lives -= 1;
            player->reset();
            player->set_loc(startx, starty);
            finished_level = true;
        }
        else
        {
            lives = 6;
            state = GAME_OVER;
        }
        Mix_PlayChannel(-1, die, 0);
    }
    
    animation_elapsed += elapsed;
    if (current_anim_index == 0)
    {
        if (animation_elapsed > 2.0f)
        {
            current_anim_index = 2;
            animation_elapsed = 0.0f;
        }
    }
    else if (current_anim_index == 2)
    {
        if (animation_elapsed > 0.25f)
        {
            current_anim_index = 0;
            animation_elapsed = 0.0f;
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
            render_game_over_screen();
        }
        else
        {
            build_map();
            player->render(program, model_matrix, current_anim_index);
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
    
    render();
    return done;
}