//
//  Entity.hpp
//  NYUCodebase
//

#ifndef Entity_hpp
#define Entity_hpp

#include "Utils.hpp"

enum BLOCK_TYPE
{
    GREEN,
    RED
};

class Entity
{
protected:
    float height;
    float width;
    float pos_x;
    float pos_y;
    float velocity_x;
    float velocity_y;
    Utilities::SheetSprite sprite;
    
private:
    float acceleration_x;
    float acceleration_y;
    float gravity;
    float friction;
    float jump_amount;
    bool collided_top;
    bool collided_bottom;
    bool collided_left;
    bool collided_right;
    bool jumped;
    
public:
    Entity();
    Entity(float x, float y, float ht, float wd, float vx,
           float vy, float ax, float ay, float gr);
    
    void set_sprite(GLuint texture_id, float scount_x, float scount_y);
    
    float get_height();
    float get_width();
    float get_pos_x();
    float get_pos_y();
    float get_vel_x();
    float get_vel_y();
    float get_acc_x();
    float get_acc_y();
    float get_gravity();
    bool has_jumped();
    
    void set_loc(float x, float y);
    bool is_colliding_with(Entity *e);
    void bounce_off_of(Entity *e);
    void jump();
    void move(float accel);
    void update_size(float amount);
    void reset();
    
    void move_y(float elapsed);
    bool is_colliding_on_y_with(Entity *e);
    void move_x(float elapsed);
    bool is_colliding_on_x_with(Entity *e);
    
    void update(float elapsed);
    void render(ShaderProgram *program, Matrix &model_matrix, int index);
};

class Enemy : public Entity
{
public:
    Enemy();
    Enemy(float x, float y, float ht, float wd, float vx = 0.0f, float vy = 0.0f);
    void move(float vx, float vy);
};

class Block : public Entity
{
private:
    BLOCK_TYPE type;
public:
    Block();
    Block(float x, float y, float ht, float wd, BLOCK_TYPE bt, float vx = 0.0f, float vy = 0.0f);
    BLOCK_TYPE get_type();
};

class Coin : public Entity
{
public:
    Coin();
    Coin(float x, float y, float diam);
};

#endif /* Entity_hpp */
