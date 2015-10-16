//
//  Entity.h
//  NYUCodebase
//

#ifndef Entity_h
#define Entity_h

#include "Utils.h"
#include "Bullet.h"

#include <vector>

class Entity
{
private:
    float height;
    float width;
    float position_x;
    float position_y;
    EntityType type;
    bool alive;
    
public:
    Entity();
    ~Entity();
    
    Entity(float ht, float wd, float x, float y, EntityType t);
    Entity(const Entity &e);
    
    float get_height() const;
    float get_width() const;
    float get_pos_x() const;
    float get_pos_y() const;
    EntityType get_type() const;
    bool is_alive() const;
    
    void move_x(float distance);
    void move_y(float distance);
    
    void die();
    void shoot(std::vector<Bullet*> &bullets);
    
    void draw(GLuint texture_id, ShaderProgram *program, Matrix &model_matrix);
};

#endif
