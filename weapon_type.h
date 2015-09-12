/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
