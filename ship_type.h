#ifndef ship_type_h
#define ship_type_h

#include <string>

class Ship_Type{
public:

    std::string name;
    std::string sprite;

    double health;
    double armor;

    //N/s
    double thrust;
    double angular_thrust;
    double stabilizer;
    double angular_stabilizer;

    Ship_Type();
};

#endif
