#ifndef weapon_h
#define weapon_h

#include "collision.h"
#include "vector.h"

#include <string>

class Weapon{
public:

    std::string type;

    int cooldown;

    Weapon(std::string get_type);

    double get_targeting_radius();

    void cool();
    bool cooled();

    void fire(std::string projectile,Collision_Circ parent_circle,double angle,Vector parent_velocity,double parent_angular_velocity);
};

#endif
