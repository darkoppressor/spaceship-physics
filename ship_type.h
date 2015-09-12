/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_type_h
#define ship_type_h

#include "coords.h"

#include <string>

class Ship_Type{
public:

    std::string name;
    std::string sprite;

    Coords thruster_up;
    Coords thruster_down;
    Coords thruster_left;
    Coords thruster_right;

    double health;
    double armor;

    //kg/m^3
    double density;

    //N/s
    double thrust;
    double angular_thrust;
    double brake;
    double stabilizer;
    double angular_stabilizer;

    Ship_Type();
};

#endif
