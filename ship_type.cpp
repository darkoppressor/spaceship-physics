/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship_type.h"

using namespace std;

Ship_Type::Ship_Type(){
    name="";
    sprite="";

    health=0.0;
    armor=0.0;

    density=0.0;

    thrust=0.0;
    angular_thrust=0.0;
    brake=0.0;
    stabilizer=0.0;
    angular_stabilizer=0.0;
}
