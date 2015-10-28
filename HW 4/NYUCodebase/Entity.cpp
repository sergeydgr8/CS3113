//
//  Entity.cpp
//  NYUCodebase
//

#include "Entity.h"

Entity::Entity()
{
}

void Entity::set_sprite(unsigned int texture_id, float u, float v, float width, float height, float size)
{
    sprite = Utilities::SheetSprite(texture_id, u, v, width, height, size);
}

float Entity::get_x() const { return x; }
float Entity::get_y() const { return y; }
float Entity::get_height() const { return height; }
float Entity::get_width() const { return width; }
float Entity::get_vel_x() const { return velocity_x; }
float Entity::get_vel_y() const { return velocity_y; }
float Entity::get_acc_x() const { return acceleration_x; }
float Entity::get_acc_y() const { return acceleration_y; }
bool Entity::is_static() { return static_entity; }
bool Entity::is_colliding() { return (collided_top || collided_bottom || collided_left || collided_right); }

void Entity::Update(float elapsed)
{
    collided_top = false;
    collided_bottom = false;
    collided_left = false;
    collided_right = false;

    x += velocity_x * elapsed;
    y += velocity_y * elapsed;

    velocity_x += acceleration_x * elapsed;
    velocity_y += acceleration_y * elapsed;

    if (velocity_x > 0.0f) velocity_x -= friction * elapsed;
    else if (velocity_x < 0.0f) velocity_x += friction * elapsed;

    velocity_y -= gravity * elapsed;
}

void Entity::Render(ShaderProgram *program, Matrix &model_matrix)
{
    sprite.Draw(program, model_matrix, x, y);
}

bool Entity::collides_with(Entity *entity)
{
    float this_top = y + (height / 2.0f);
    float this_bottom = y - (height / 2.0f);
    float this_left = x - (width / 2.0f);
    float this_right = x + (width / 2.0f);

    float rhs_top = entity->get_y() + (entity->get_height() / 2.0f);
    float rhs_bottom = entity->get_y() - (entity->get_height() / 2.0f);
    float rhs_left = entity->get_x() - (entity->get_width() / 2.0f);
    float rhs_right = entity->get_x() + (entity->get_width() / 2.0f);

    float penetration = 0.0f;

    // top & bottom
    if (fabs(this_left - rhs_left) < entity->get_width() && fabs(this_right - rhs_right) < entity->get_width())
    {
        if (this_bottom <= rhs_top && this_top > rhs_top)
        {
            penetration = fabs(this_bottom - rhs_top);
            y += penetration + 0.001f;
            collided_bottom = true;
        }
        else if (this_top >= rhs_bottom && this_bottom < rhs_bottom)
        {
            penetration = fabs(this_top - rhs_bottom);
            y -= penetration + 0.001f;
            collided_top = true;
        }
    }

    // right & left
    if (fabs(this_top - rhs_top) < entity->get_height() && fabs(this_bottom - rhs_bottom) < entity->get_height())
    {
        if (this_left <= rhs_right && this_right > rhs_right)
        {
            penetration = fabs(this_left - rhs_right);
            x += penetration + 0.001f;
            collided_left = true;
        }
        else if (this_right >= rhs_left && this_left < rhs_left)
        {
            penetration = fabs(this_right - rhs_left);
            x -= penetration + 0.001f;
            collided_right = true;
        }
    }

    return is_colliding();
}