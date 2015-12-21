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
    gravity = 50.0f;
}

Entity::Entity(float x, float y, float ht, float wd, float vx,
               float vy, float ax, float ay, float gr) :
               height(ht), width(wd), pos_x(x), pos_y(y), velocity_x(vx),
               velocity_y(vy), acceleration_x(ax), acceleration_y(ay),
               gravity(gr) { friction = 3.0f; jump_amount = height;}

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
    float top = pos_y + (height);
    float bottom = pos_y - (height);
    float left = pos_x - (width);
    float right = pos_x + (width);
    
    float etop = e->get_pos_y() + (e->get_height());
    float ebottom = e->get_pos_y() - (e->get_height());
    float eleft = e->get_pos_x() - (e->get_width());
    float eright = e->get_pos_x() + (e->get_width());
    
    if (bottom <= etop && bottom > ebottom)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright)
            || (left <= eleft && right >= eright))
            return true;
    }
    else if (top >= ebottom && top < etop)
    {
        if ((left >= eleft && left <= eright) || (right >= eleft && right <= eright)
            || (left <= eleft && right >= eright))
            return true;
    }
    if (left <= eright && left > eleft)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom)
            || (top >= etop && bottom <= ebottom))
            return true;
    }
    else if (right >= eleft && right < eright)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom)
            || (top >= etop && bottom <= ebottom))
            return true;
    }
    
    return false;
}

void Entity::bounce_off_of(Entity *e)
{
    if ((collided_top || collided_bottom))
    {
        velocity_y = 0.0f;
        acceleration_y = 0.0f;
    }
    if ((collided_left || collided_right))
    {
        velocity_x = 0.0f;
        acceleration_x = 0.0f;
    }
    
    float penetration = 0.0f;
    
    if (collided_bottom)
    {
        penetration = fabs(pos_y - e->get_pos_y() - height - e->get_height());
        pos_y += (penetration + 0.0001f);
    }
    else if (collided_top)
    {
        penetration = fabs(e->get_pos_y() - pos_y - height - e->get_height());
        pos_y -= (penetration + 0.0001f);
    }
    if (collided_left)
    {
        penetration = fabs(pos_x - e->get_pos_x() - width - e->get_width());
        pos_x += (penetration + 0.0001f);
    }
    else if (collided_right)
    {
        penetration = fabs(e->get_pos_x() - pos_x - width - e->get_width());
        pos_x -= (penetration + 0.0001f);
    }
}

void Entity::jump()
{
    //velocity_y = jump_amount;
    velocity_y = 4.3f;
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
    sprite.UpdateSize(width, height);
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
    sprite.UpdateSize(width, height);
}

void Entity::reset_collisions()
{
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;
}

void Entity::move_y(float elapsed)
{
    pos_y += velocity_y * elapsed;
    velocity_y += (acceleration_y - gravity) * elapsed;
}

bool Entity::is_colliding_on_y_with(Entity *e)
{
    collided_bottom = false;
    collided_top = false;
    float top = pos_y + (height);
    float bottom = pos_y - (height);
    float left = pos_x - (width);
    float right = pos_x + (width);
    
    float etop = e->get_pos_y() + (e->get_height());
    float ebottom = e->get_pos_y() - (e->get_height());
    float eleft = e->get_pos_x() - (e->get_width());
    float eright = e->get_pos_x() + (e->get_width());
    
    if (bottom < etop && bottom > ebottom)
    {
        if ((left > eleft && left < eright) || (right > eleft && right < eright)
            || (left < eleft && right > eright))
        {
            collided_bottom = true;
            jumped = false;
        }
    }
    else if (top > ebottom && top < etop)
    {
        if ((left > eleft && left < eright) || (right > eleft && right < eright)
            || (left <= eleft && right >= eright))
            collided_top = true;
    }
    
    return (collided_bottom || collided_top);
}

void Entity::move_x(float elapsed)
{
    pos_x += velocity_x * elapsed;
    velocity_x += acceleration_x * elapsed;
    
    if (velocity_x > 0.0f) velocity_x -= friction * elapsed;
    else if (velocity_x < 0.0f) velocity_x += friction * elapsed;
}

bool Entity::is_colliding_on_x_with(Entity *e)
{
    collided_left = false;
    collided_right = false;
    float top = pos_y + (height);
    float bottom = pos_y - (height);
    float left = pos_x - (width);
    float right = pos_x + (width);
    
    float etop = e->get_pos_y() + (e->get_height());
    float ebottom = e->get_pos_y() - (e->get_height());
    float eleft = e->get_pos_x() - (e->get_width());
    float eright = e->get_pos_x() + (e->get_width());
    
    if (left <= eright && left > eleft)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom)
            || (top >= etop && bottom <= ebottom))
            collided_left = true;
    }
    else if (right >= eleft && right < eright)
    {
        if ((top <= etop && top >= ebottom) || (bottom <= etop && bottom >= ebottom)
            || (top >= etop && bottom <= ebottom))
            collided_right = true;
    }
    
    return (collided_left || collided_right);
}

void Entity::update(float elapsed)
{
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;
    
    pos_x += velocity_x * elapsed;
    velocity_x += acceleration_x * elapsed;
    
    
    pos_y += velocity_y * elapsed;
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

Enemy::Enemy(float x, float y, float ht, float wd, ENEMY_TYPE tp, float vx, float vy)
{
    pos_x = x;
    pos_y = y;
    original_x = x;
    original_y = y;
    height = ht;
    width = wd;
    type = tp;
    velocity_x = vx;
    velocity_y = vy;
    alive = true;
}

void Enemy::move(float vx, float vy)
{
    velocity_x = vx;
    velocity_y = vy;
}

ENEMY_TYPE Enemy::get_type()
{
    return type;
}

bool Enemy::is_alive()
{
    return alive;
}

void Enemy::die()
{
    alive = false;
}

void Enemy::switch_direction()
{
    velocity_x *= -1;
    velocity_y *= -1;
}

void Enemy::move_x(float elapsed)
{
    pos_x += velocity_x * elapsed;
}

float Enemy::get_original_x()
{
    return original_x;
}

float Enemy::get_original_y()
{
    return original_y;
}

void Enemy::move_towards(Entity *e, float elapsed)
{
    float magnitude = sqrtf(powf(fabsf(pos_y - e->get_pos_y()), 2) +
                            powf(fabsf(pos_x - e->get_pos_x()), 2));
    velocity_y = (pos_y - e->get_pos_y()) * 0.5f / magnitude;
    velocity_x = (pos_x - e->get_pos_x()) * 0.5f / magnitude;
    
    pos_y -= velocity_y * elapsed;
    pos_x -= velocity_x * elapsed;
}

//
// Block definitions
//

Block::Block(float x, float y, float ht, float wd, BLOCK_TYPE bt, float vx, float vy)
{
    pos_x = x;
    pos_y = y;
    height = ht;
    width = wd;
    type = bt;
    velocity_x = vx;
    velocity_y = vy;
}

BLOCK_TYPE Block::get_type()
{
    return type;
}

//
// Coin definitions
//

Coin::Coin()
{
    pos_x = 0.0f;
    pos_y = 0.0f;
    height = 1.0f;
    width = 1.0f;
}

Coin::Coin(float x, float y, float radius)
{
    pos_x = x;
    pos_y = y;
    height = radius;
    width = radius;
}

//
// Growblock definitions
//

Growblock::Growblock()
{
    pos_x = 0.0f;
    pos_y = 0.0f;
    height = 0.1f;
    width = 0.1f;
    active = true;
}

Growblock::Growblock(float x, float y)
{
    pos_x = x;
    pos_y = y;
    height = 0.1f;
    width = 0.1f;
    active = true;
}

void Growblock::hit()
{
    active = false;
    pos_x = -20.0f;
    pos_y = -20.0f;
    update_size(-0.1f);
}

bool Growblock::is_active()
{
    return active;
}