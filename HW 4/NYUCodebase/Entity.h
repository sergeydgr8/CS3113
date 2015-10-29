//
//  Entity.h
//  NYUCodebase
//

#ifndef Entity_h
#define Entity_h

#include "Utils.h"

enum EntityType
{
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_COIN,
    ENTITY_BLOCK
};

class Entity
{
private:
    float height;
    float width;

    float x;
    float y;

    float velocity_x;
    float velocity_y;

    float acceleration_x;
    float acceleration_y;

    float friction;
    float gravity = -9.81f;

    bool static_entity;

    bool collided_top = false;
    bool collided_bottom = false;
    bool collided_left = false;
    bool collided_right = false;

    EntityType entity_type;

    Utilities::SheetSprite sprite;

protected:
    bool is_colliding();

public:
    Entity();
    Entity(float x, float y, float height, float width, bool static_entity, EntityType entity_type);

    void set_sprite(unsigned int texture_id, float u, float v, float width, float height, float size);

    float get_x() const;
    float get_y() const;
    float get_height() const;
    float get_width() const;
    float get_vel_x() const;
    float get_vel_y() const;
    float get_acc_x() const;
    float get_acc_y() const;
    bool is_static();
    bool is_standing();

    void move_x(float vel);
    void move_y(float vel);

    void Update(float elapsed);
    void Render(ShaderProgram *program, Matrix &model_matrix);
    bool collides_with(Entity *entity);
};

#endif
