#ifndef projectile_type_h
#define projectile_type_h

#include <string>

class Projectile_Type{
public:

    std::string name;
    std::string sprite;
    std::string sound_thrust;

    double health;

    //kg/m^3
    double density;

    //N/s
    double thrust;

    Projectile_Type();
};

#endif
