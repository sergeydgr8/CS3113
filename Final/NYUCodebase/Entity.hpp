//
//  Entity.hpp
//  NYUCodebase
//

#ifndef Entity_hpp
#define Entity_hpp

#include "Utils.hpp"

enum EntityType
{
    PLAYER,
    ENEMY,
    COIN,
    BLOCK
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
    EntityType type;
    Utilities::SheetSprite sprite;
    bool static_entity;
    
private:
    float acceleration_x;
    float acceleration_y;
    float gravity;
    float friction;
    bool collided_top;
    bool collided_bottom;
    bool collided_left;
    bool collided_right;
    
public:
    Entity();
    Entity(float ht, float wd, float x, float y, float vx,
           float vy, float ax, float ay, float gr, EntityType tp);
    
    void set_sprite(GLuint texture_id, float u, float v,
                    float w, float h, float sz);
    
    float get_height();
    float get_width();
    float get_pos_x();
    float get_pos_y();
    float get_vel_x();
    float get_vel_y();
    float get_acc_x();
    float get_acc_y();
    float get_gravity();
    EntityType get_type();
    bool is_static();
    
    void set_loc(float x, float y);
    bool is_colliding_with(Entity *e);
    void jump();
    void move(float accel);
    
    void update(float elapsed);
    void render(ShaderProgram *program, Matrix &model_matrix, int index);
};

class Block : public Entity
{
public:
    Block();
    Block(float x, float y, float tile_size);
};

#endif /* Entity_hpp */
