#ifndef projectile_h
#define projectile_h

#include "object.h"
#include "coords.h"
#include "vector.h"

class Projectile: public Object{
public:

    std::string type;

    int counter_sound_thrust;

    Projectile(std::string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity);

    double get_health_max();
    double get_thrust();
    std::string get_sound_thrust();

    void apply_thrust();

    void accelerate();
    void movement(uint32_t index);
    void collide_with_ship(uint32_t index);

    void take_damage(Vector damage_force);
    void die();

    void animate();
};

#endif
