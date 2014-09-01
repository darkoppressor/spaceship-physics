#ifndef weapon_type_h
#define weapon_type_h

#include <string>

class Weapon_Type{
public:

    std::string name;
    std::string sound;

    //ms
    double cooldown;

    //pixels
    double targeting_radius;

    //N
    double force;

    Weapon_Type();
};

#endif
