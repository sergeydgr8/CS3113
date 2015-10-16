//
//  Bullet.cpp
//  NYUCodebase
//

#include "Bullet.h"

Bullet::Bullet(float x, float y, EntityType src) : position_x(x),
position_y(y), source_type(src), time_left(1.5f)
{
    if (source_type == PLAYER)
        position_y += 1.0f;
    else position_y -= 1.0f;
}

float Bullet::get_pos_x() const { return position_x; }
float Bullet::get_pos_y() const { return position_y; }
float Bullet::get_diameter() const { return diameter; }
float Bullet::get_time_left() const { return time_left; }
EntityType Bullet::get_type() const { return source_type; }

void Bullet::update(float elapsed)
{
    if (source_type == PLAYER)
        position_y += elapsed * 15.0f;
    else position_y -= elapsed * 15.0f;

    time_left -= elapsed;
}

void Bullet::draw(GLuint texture_id, ShaderProgram *program, Matrix &model_matrix)
{
    float half_diam = diameter / 2.0f;

    float vertices[] =
    {
        position_x - half_diam, position_y - half_diam,
        position_x + half_diam, position_y + half_diam,
        position_x - half_diam, position_y + half_diam,
        position_x - half_diam, position_y - half_diam,
        position_x + half_diam, position_y - half_diam,
        position_x + half_diam, position_x + half_diam
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