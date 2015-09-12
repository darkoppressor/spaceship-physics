/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
    double angular_velocity;
    Vector net_force;
    double net_angular_force;
    Vector acceleration;
    double angular_acceleration;

    double health;

    Sprite sprite;

    void setup(double get_mass,Collision_Circ get_circle,Vector get_velocity,double get_angular_velocity,double get_health,std::string get_sprite);

    double get_volume();
    double get_density();

    Vector get_momentum();
    double get_angular_momentum();

    bool is_alive();

    void accelerate();
    void movement();
    void gravitate(uint32_t index);

    void animate();
    void render();
};

#endif
