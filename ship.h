#ifndef ship_h
#define ship_h

#include "object.h"
#include "coords.h"
#include "vector.h"
#include "weapon.h"

#include <vector>

class Ship: public Object{
public:

    std::string type;

    bool thrust_left;
    bool thrust_up;
    bool thrust_right;
    bool thrust_down;
    bool braking;

    bool stabilize_left;
    bool stabilize_right;
    bool stabilize_brake;
    Vector stabilize_brake_vector;

    int counter_thrust_left;
    int counter_thrust_up;
    int counter_thrust_right;
    int counter_thrust_down;
    int counter_stabilize_left;
    int counter_stabilize_right;
    int counter_stabilize_brake;
    int counter_sound_thrust;
    int counter_sound_stabilize;

    bool stabilizer_positional;
    bool stabilizer_angular;

    double armor;

    std::string faction;

    std::vector<Weapon> weapons;

    Ship(std::string get_type,double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,std::string get_faction,std::vector<std::string> get_weapons);

    double get_health_max();
    double get_armor_max();
    double get_thrust();
    double get_angular_thrust();
    double get_brake();
    double get_stabilizer();
    double get_angular_stabilizer();
    Coords get_thruster_up();
    Coords get_thruster_down();
    Coords get_thruster_left();
    Coords get_thruster_right();

    void reset_thrust_input();
    //Stabilizer "input" is automated and used for creating effects
    void reset_stabilizer_input();

    void brake(double stabilizer);
    void apply_thrust();

    void ai();
    void fire_weapons();
    void accelerate();
    void movement(uint32_t index);
    void collide_with_ship(uint32_t index);

    void take_damage(Vector damage_force);
    void die();

    void create_thrust_effect(std::string direction,double scale=1.0);

    void animate();
};

#endif
