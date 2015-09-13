/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef effect_transient_h
#define effect_transient_h

#include "collision.h"
#include "vector.h"
#include "sprite.h"

class Effect_Transient{
public:

    Collision_Circ circle;

    Vector velocity;
    double angular_velocity;

    Sprite sprite;

    double scale;
    double angle;

    bool exists;

    Effect_Transient(double x,double y,Vector get_velocity,double get_angular_velocity,double get_scale,double get_angle,std::string sprite_name);

    void movement();

    void animate();
    void render();
};

#endif
