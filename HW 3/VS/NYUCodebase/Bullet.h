//
//  Bullet.h
//  NYUCodebase
//

#ifndef Bullet_h
#define Bullet_h

#include "Utils.h"

class Bullet
{
private:
    float diameter = 0.5f;
    float position_x;
    float position_y;
    EntityType source_type;
    float time_left;

public:
    Bullet(float x, float y, EntityType src);

    float get_pos_x() const;
    float get_pos_y() const;
    float get_diameter() const;
    float get_time_left() const;
    EntityType get_type() const;
    void update(float elapsed);
    void draw(GLuint texture_id, ShaderProgram *program, Matrix &model_matrix);
};

#endif
