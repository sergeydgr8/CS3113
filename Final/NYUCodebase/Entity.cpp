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
}

Entity::Entity(float x, float y, float ht, float wd, float vx,
               float vy, float ax, float ay, float gr) :
               height(ht), width(wd), pos_x(x), pos_y(y), velocity_x(vx),
               velocity_y(vy), acceleration_x(ax), acceleration_y(ay),
               gravity(gr) { friction = 3.0f; jump_amount = height * 20.0f;}

void Entity::set_sprite(GLuint texture_id, float scount_x, float scount_y)
{
    sprite = Utilities::SheetSprite(texture_id, scount_x, scount_y, width, height, height / 0.5f);
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
bool Entity::has_jumped() { return jumped; }

void Entity::set_loc(float x, float y)
{
    pos_x = x;
    pos_y = y;
}

bool Entity::is_colliding_with(Entity *e)
{
    float top = pos_y + (height / 2.0f);
    float bottom = pos_y - (height / 2.0f);
    float left = pos_x - (width / 2.0f);
    float right = pos_y + (width / 2.0f);
    
    float etop = e->get_pos_y() + (e->get_height() / 2.0f);
    float ebottom = e->get_pos_y() - (e->get_height() / 2.0f);
    float eleft = e->get_pos_x() - (e->get_width() / 2.0f);
    float eright = e->get_pos_x() + (e->get_width() / 2.0f);
    
    /*if (bottom < etop && fabs(left - eleft) < e->get_width() &&
        fabs(right - eright) < e->get_width())
    {
        collided_bottom = true;
        jumped = false;
    }
    
    else if (top > ebottom && fabs(left - eleft) < e->get_width() &&
             fabs(right - eright) < e->get_width())
    {
        collided_top = true;
    }
    
    if (left < eright && fabs(top - etop) < e->get_height() &&
        fabs(bottom - ebottom) < e->get_height())
    {
        collided_left = true;
    }
    
    else if (right > eleft && fabs(top - etop) < e->get_height() &&
             fabs(bottom - ebottom) < e->get_height())
    {
        collided_right = true;
    }*/
    
    
    if (bottom <= etop && bottom > ebottom)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright))
        {
            collided_bottom = true;
            jumped = false;
        }
    }
    else if (top >= ebottom && top < etop)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright))
            collided_top = true;
    }
    else if (left <= eright && left > eleft)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom))
            collided_left = true;
    }
    else if (right >= eleft && right < eright)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom))
            collided_right = true;
    }
    
    return (collided_top || collided_bottom ||
            collided_left || collided_right);
}

void Entity::bounce_off_of(Entity *e)
{
    float top = pos_y + (height / 2.0f);
    float bottom = pos_y - (height / 2.0f);
    float left = pos_x - (width / 2.0f);
    float right = pos_y + (width / 2.0f);
    
    float etop = e->get_pos_y() + (e->get_height() / 2.0f);
    float ebottom = e->get_pos_y() - (e->get_height() / 2.0f);
    float eleft = e->get_pos_x() - (e->get_width() / 2.0f);
    float eright = e->get_pos_x() + (e->get_width() / 2.0f);
    
    float penetration = 0.0f;
    
    if (collided_bottom)
    {
        penetration = fabs(bottom - etop);
        pos_y += (penetration + 0.0001f);
    }
    else if (collided_top)
    {
        penetration = fabs(top - ebottom);
        pos_y -= (penetration + 0.0001f);
    }
    else if (collided_left)
    {
        penetration = fabs(left - eright);
        pos_x += (penetration + 0.0001f);
    }
    else if (collided_right)
    {
        penetration = fabs(right - eleft);
        pos_x -= (penetration + 0.0001f);
    }
    
    /*if (bottom <= etop && bottom > ebottom)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright))
        {
            penetration = fabs(pos_y - e->get_pos_y() -
                               (height / 2.0f) - (e->get_height() / 2.0f));
            pos_y += (penetration + 0.0001f);
        }
    }
    else if (top >= ebottom && top < etop)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright))
        {
            penetration = fabs(e->get_pos_y() - pos_y -
                               (height / 2.0f) - (e->get_height() / 2.0f));
            pos_y -= (penetration + 0.0001f);
        }
    }
    if (left <= eright && left > eleft)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom))
        {
            penetration = fabs(pos_x - e->get_pos_x() -
                               (width / 2.0f) - (e->get_width() / 2.0f));
            pos_x += (penetration + 0.0001f);
        }
    }
    else if (right >= eleft && right < eright)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom))
        {
            penetration = fabs(e->get_pos_x() - pos_x -
                               (width / 2.0f) - (e->get_width() / 2.0f));
            pos_x -= (penetration + 0.0001f);
        }
    }*/
    
    if ((collided_top || collided_bottom))
    {
        velocity_y = 0.0f;
        acceleration_y = 0.0f;
    }
    if ((collided_left || collided_right))
        velocity_x = 0.0f;
}

void Entity::jump()
{
    velocity_y = jump_amount;
    jumped = true;
}

void Entity::move(float accel)
{
    acceleration_x = accel;
}

void Entity::update_size(float amount)
{
    float ratio = width / height;
    width += amount;
    height += (amount / ratio);
}

void Entity::reset()
{
    acceleration_x = 0.0f;
    acceleration_y = 0.0f;
    velocity_x = 0.0f;
    velocity_y = 0.0f;
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;
    height = 0.25f;
    width = 0.25f;
    jumped = false;
    jump_amount = height * 20.0f;
}

void Entity::update(float elapsed)
{
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;
    
    pos_x += velocity_x * elapsed;
    pos_y += velocity_y * elapsed;
    
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

int Block::get_type()
{
    return block_type;
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