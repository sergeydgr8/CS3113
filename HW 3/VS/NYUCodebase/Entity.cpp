//
//  Entity.cpp
//  NYUCodebase
//

#include "Entity.h"

Entity::Entity()
{
    height = 1.0f;
    width = 1.0f;
    position_x = 0.0f;
    position_y = 0.0f;
    type = PLAYER;
    alive = true;
}

Entity::Entity(float ht, float wd, float x, float y, EntityType t) :
height(ht), width(wd), position_x(x), position_y(y), alive(true) {}

Entity::Entity(const Entity &e)
{
    this->height = e.get_height();
    this->width = e.get_width();
    this->position_x = e.get_pos_x();
    this->position_y = e.get_pos_y();
    this->type = e.get_type();
    this->alive = e.is_alive();
}

Entity::~Entity() { }

float Entity::get_height() const { return height; }
float Entity::get_width() const { return width; }
float Entity::get_pos_x() const { return position_x; }
float Entity::get_pos_y() const { return position_y; }
EntityType Entity::get_type() const { return type; }
bool Entity::is_alive() const { return alive; }

void Entity::move_x(float distance) { position_x += distance; }
void Entity::move_y(float distance) { position_y += distance; }

void Entity::die() { alive = false; }
void Entity::shoot(std::vector<Bullet*> &bullets) { bullets.push_back(new Bullet(position_x, position_y, type)); }

void Entity::draw(GLuint texture_id, ShaderProgram *program, Matrix &model_matrix)
{
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    float vertices[] =
    {
        position_x - half_width, position_y - half_height,
        position_x + half_width, position_y + half_height,
        position_x - half_width, position_y + half_height,
        position_x - half_width, position_y - half_height,
        position_x + half_width, position_y - half_height,
        position_x + half_width, position_y + half_height
    };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    float texture_coords[] =
    {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    model_matrix.identity();
    program->setModelMatrix(model_matrix);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}