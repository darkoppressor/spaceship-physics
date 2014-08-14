#ifndef object_h
#define object_h

#include "sprite.h"
#include "collision.h"
#include "vector.h"

class Object{
public:

    double mass;
    Collision_Circ circle;
    double angle;

    Vector velocity;
    Vector net_force;

    bool thrust_left;
    bool thrust_up;
    bool thrust_right;
    bool thrust_down;

    Sprite sprite;

    Object(double get_mass,Collision_Circ get_circle,double get_angle,Vector get_velocity,std::string get_sprite);

    void reset_thrust_input();

    void apply_thrust();
    void accelerate();
    void movement(uint32_t index);
    void collide(uint32_t index);
    void gravitate(uint32_t index);

    void animate();
    void render();
};

#endif
