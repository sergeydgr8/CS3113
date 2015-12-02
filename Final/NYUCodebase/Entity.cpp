//
//  Entity.cpp
//  NYUCodebase
//

#include "Entity.hpp"

//
// General Entity definitions
//

Entity::Entity()
{
    height = 1.0f;
    width = 1.0f;
    pos_x = 0.0f;
    pos_y = 0.0f;
    velocity_x = 0.0f;
    velocity_y = 0.0f;
    acceleration_x = 0.0f;
    acceleration_y = 0.0f;
    gravity = 9.81f;
    static_entity = true;
}

Entity::Entity(float ht, float wd, float x, float y, float vx,
               float vy, float ax, float ay, float gr) :
               height(ht), width(wd), pos_x(x), pos_y(y), velocity_x(vx),
               velocity_y(vy), acceleration_x(ax), acceleration_y(ay),
               gravity(gr) { friction = 5.0f; }

void Entity::set_sprite(GLuint texture_id, float scount_x, float scount_y,
                        float w, float h, float sz)
{
    sprite = Utilities::SheetSprite(texture_id, scount_x, scount_y, w, h, sz);
}

float Entity::get_height() { return height; }
float Entity::get_width() { return width; }
float Entity::get_pos_x() { return pos_x; }
float Entity::get_pos_y() { return pos_y; }
float Entity::get_vel_x() { return velocity_x; }
float Entity::get_vel_y() { return velocity_y; }
float Entity::get_acc_x() { return acceleration_x; }
float Entity::get_acc_y() { return acceleration_y; }
float Entity::get_gravity() { return gravity; }
bool Entity::is_static() { return static_entity; }

void Entity::set_loc(float x, float y)
{
    pos_x = x;
    pos_y = y;
}

bool Entity::is_colliding_with(Entity *e)
{
    float top = pos_y + (height / 2);
    float bottom = pos_y - (height / 2);
    float left = pos_x - (width / 2);
    float right = pos_y + (width / 2);
    
    float etop = e->get_pos_y() + (e->get_height() / 2);
    float ebottom = e->get_pos_y() - (e->get_height() / 2);
    float eleft = e->get_pos_x() - (e->get_width() / 2);
    float eright = e->get_pos_x() + (e->get_width() / 2);
    
    float penetration = 0.0f;
    
    if (bottom <= etop)
    {
        if (fabs(left - eleft) < e->width &&
            fabs(right - eright) < e->width)
        {
            collided_bottom = true;
            penetration = fabs(pos_y + e->get_pos_y() -
                               (height / 2) - (e->get_height() / 2));
            pos_y += (penetration + 0.0001f);
        }
    }
    else if (top >= ebottom)
    {
        if (fabs(left - eleft) < e->width &&
            fabs(right - eright) < e->width &&
            bottom < etop)
        {
            collided_top = true;
            penetration = fabs(e->get_pos_y() + pos_y -
                               (height / 2) - (e->get_height() / 2));
            pos_y -= (penetration + 0.0001f);
        }
    }
    if (left <= eright)
    {
        if (fabs(top - etop) < e->height &&
            fabs(bottom - ebottom) < e->height)
        {
            collided_left = true;
            penetration = fabs(pos_x - e->get_pos_x() -
                               (width / 2) - (e->get_width() / 2));
            pos_x += (penetration + 0.0001f);
        }
    }
    else if (right >= eleft)
    {
        if (fabs(top - etop) < e->height &&
            fabs(bottom - ebottom) < e->height &&
            left < eright)
        {
            collided_right = true;
            penetration = fabs(e->get_pos_x() - pos_x -
                               (width / 2) - (e->get_width() / 2));
            pos_x -= (penetration + 0.0001f);
        }
    }
    
    if ((collided_top || collided_bottom))
        velocity_y = 0.0f;
    if ((collided_left || collided_right))
        velocity_x = 0.0f;
    
    return (collided_top || collided_bottom ||
            collided_left || collided_right);
}

void Entity::jump()
{
    if (collided_bottom)
        velocity_y = 5.0f;
}

void Entity::move(float accel)
{
    acceleration_x = accel;
}

void Entity::update(float elapsed)
{
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;
    
    this->set_loc(pos_x + (velocity_x * elapsed),
                  pos_y + (velocity_y * elapsed));
    
    velocity_x += acceleration_x * elapsed;
    velocity_y += (acceleration_y - gravity) * elapsed;
    
    if (velocity_x > 0.0f) velocity_x -= friction * elapsed;
    else if (velocity_x < 0.0f) velocity_x += friction * elapsed;
}

void Entity::render(ShaderProgram *program, Matrix &model_matrix, int index)
{
    sprite.Draw(program, model_matrix, index, pos_x, pos_y);
}

//
// Enemy definitions
//

Enemy::Enemy(float x, float y, float ht, float wd, float vx, float vy)
{
    pos_x = x;
    pos_y = y;
    height = ht;
    width = wd;
    velocity_x = vx;
    velocity_y = vy;
}

void Enemy::move(float vx, float vy)
{
    velocity_x = vx;
    velocity_y = vy;
}

//
// Block definitions
//

Block::Block(float x, float y, float ht, float wd, int bt, float vx, float vy)
{
    pos_x = x;
    pos_y = y;
    height = ht;
    width = wd;
    block_type = bt;
    velocity_x = vx;
    velocity_y = vy;
}

//
// Coin definitions
//

Coin::Coin(float x, float y, float diam)
{
    pos_x = x;
    pos_y = y;
    height = diam;
    width = diam;
}