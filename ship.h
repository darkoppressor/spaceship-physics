#ifndef ship_h
#define ship_h

#include "object.h"

class Ship: public Object{
public:

    std::string type;

    bool thrust_left;
    bool thrust_up;
    bool thrust_right;
    bool thrust_down;
    bool brake;

    double armor;

    std::string faction;

    Ship(std::string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,std::string get_faction);

    double get_health_max();
    double get_armor_max();
    double get_thrust();
    double get_angular_thrust();
    double get_stabilizer();
    double get_angular_stabilizer();

    void reset_thrust_input();

    void apply_thrust();

    void ai();
    void accelerate();
    void movement(uint32_t index);
    void collide_with_ship(uint32_t index);

    void take_damage(Vector damage_force);
};

#endif
